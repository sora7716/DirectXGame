#include "TextureManager.h"
#include "engine/base/DirectXBase.h"
#include "engine/base/SRVManager.h"
#include <cassert>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")
using namespace Microsoft::WRL;
//初期化
TextureManager* TextureManager::instance = nullptr;
//ImGuiで0番目を使用するため、1番目から使用
uint32_t TextureManager::kSRVIndexTop = 1;

//インスタンスのゲッター
TextureManager* TextureManager::GetInstance() {
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new TextureManager();
	}
	return instance;
}

void TextureManager::Initialize(DirectXBase* directXBase, SRVManager* srvManager) {
	assert(directXBase);//Nullチェック
	directXBase_ = directXBase;
	//SRVの数と同数
	textureDatas_.reserve(SRVManager::kMaxSRVCount);
}

//テクスチャファイルの読み込み
void TextureManager::LoadTexture(const std::string& filePath) {
	//テクスチャファイルを読み込んでプログラムを扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = Log::ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//テクスチャデータを追加
	//textureDatas_.resize(textureDatas_.size() + 1);
	//追加したテクスチャデータの参照を取得する
	TextureData& textureData = textureDatas_[filePath];
	textureData.metadata = mipImages.GetMetadata();
	textureData.resourece = directXBase_->CreateTextureResource(textureData.metadata);
	textureData.intermediateResource = directXBase_->UploadTextureData(textureData.resourece.Get(), mipImages);//ここでエラーが起きる
	//SRVの確保
	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);
	//SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureData.metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(textureData.metadata.mipLevels);
	//設定を元にSRVを生成
	directXBase_->GetDevice()->CreateShaderResourceView(textureData.resourece.Get(), &srvDesc, textureData.srvHandleCPU);
	//読み込み済みテクスチャを検索
	if (textureDatas_.contains(filePath)) {
		//早期リターン
		return;
	}
	//テクスチャ枚数上限チェック
	assert(srvManager_->AllocateCheck());
}

//ファイルパス
uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath) {
	//読み込み済みデータを検索
	auto it = textureDatas_.find(filePath);
	if (it != textureDatas_.end()) {
		// ハッシュ値を返す
		return static_cast<uint32_t>(std::hash<std::string>{}(filePath));
	}
	assert(false && "Texture Not Found!");
	return 0;
}

// メタデータの取得
const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& filePath){
	// TODO: return ステートメントをここに挿入します
	return textureDatas_[filePath].metadata;
}

// SRVインデックスの取得
uint32_t TextureManager::GetSRVIndex(const std::string& filePath){
	return textureDatas_[filePath].srvIndex;
}

// GPUハンドルの取得
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSRVHandleGPU(const std::string& filePath){
	return textureDatas_[filePath].srvHandleGPU;
}

//終了処理
void TextureManager::Finalize() {
	delete instance;
	instance = nullptr;
	isFinalize = true;
}
