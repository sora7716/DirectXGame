#include "TextureManager.h"
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
	if (instance == nullptr) {
		instance = new TextureManager();
	}
	return instance;
}

void TextureManager::Initialize(DirectXBase* directXBase) {
	assert(directXBase);//Nullチェック
	directXBase_ = directXBase;
	//SRVの数と同数
	textureDatas_.reserve(DirectXBase::kMaxSRVCount);
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
	textureDatas_.resize(textureDatas_.size() + 1);
	//追加したテクスチャデータの参照を取得する
	TextureData& textureData = textureDatas_.back();
	textureData.filePath = filePath;
	textureData.metadata = image.GetMetadata();
	textureData.resourece = directXBase_->CreateTextureResource(textureData.metadata);
	ComPtr<ID3D12Resource> intermediateResource = directXBase_->UploadTextureData(textureData.resourece.Get(), mipImages);//ここでエラーが出る
	//テクスチャデータの要素数番号をSRVのインデックスとする
	uint32_t srvIndex = static_cast<uint32_t>(textureDatas_.size() - 1) + kSRVIndexTop;
	//CPU・GPUのSRVハンドルを取得
	textureData.srvHandleCPU = directXBase_->GetSRVCPUDescriptorHandle(srvIndex);
	textureData.srvHandleGPU = directXBase_->GetSRVGPUDescriptorHandle(srvIndex);
	//SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureData.metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(textureData.metadata.mipLevels);
	//設定を元にSRVを生成
	directXBase_->GetDevice()->CreateShaderResourceView(textureData.resourece.Get(), &srvDesc, textureData.srvHandleCPU);
	//読み込み済みテクスチャを検索
	auto it = std::find_if(
		textureDatas_.begin(),
		textureDatas_.end(),
		[&](TextureData& textureData) {return textureData.filePath == filePath; }
	);
	if (it != textureDatas_.end()) {
		//読み込み済みなら早期return
		return;
	}
	//テクスチャ枚数上限チェック
	assert(textureDatas_.size() + kSRVIndexTop < DirectXBase::kMaxSRVCount);
}

//ファイルパス
uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath) {
	//読み込み済みテクスチャデータを検索
	auto it = std::find_if(
		textureDatas_.begin(),
		textureDatas_.end(),
		[&](TextureData& textureData) {
			return textureData.filePath == filePath;
		}
	);
	if (it != textureDatas_.end()) {
		//読み込み済みなら要素番号を返す
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas_.begin(), it));
		return textureIndex;
	}
	assert(0);
	return 0;
}

//テクスチャ番号からGPUハンドルを取得
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(uint32_t textureIndex) {
	//範囲外違反チェック
	assert(textureIndex < textureDatas_.size());
	TextureData& textureData = textureDatas_[textureIndex];
	return textureData.srvHandleGPU;
}

//終了処理
void TextureManager::Finalize() {
	delete instance;
	instance = nullptr;
}
