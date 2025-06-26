#include "TextureManager.h"
#include "engine/base/DirectXBase.h"
#include "engine/base/SRVManager.h"
#include "StringUtility.h"
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

//初期化
void TextureManager::Initialize(DirectXBase* directXBase) {
	//DirectXの基盤部分を記録する
	directXBase_ = directXBase;
	//SRVの管理を記録する
	srvManager_ = SRVManager::GetInstance();
	//SRVの数と同数
	textureDatas_.reserve(SRVManager::kMaxSRVCount);
}

//テクスチャファイルの読み込み
void TextureManager::LoadTexture(const std::string& filePath) {
	//読み込み済みテクスチャを検索
	if (textureDatas_.contains(filePath)) {
		//読み込み済みなら早期リターン
		return;
	}
	//テクスチャファイルを読み込んでプログラムを扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = StringUtility::ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//テクスチャ枚数上限チェック
	assert(srvManager_->TextureLimitCheck(kSRVIndexTop));

	//追加したテクスチャデータの参照を取得する
	TextureData& textureData = textureDatas_[filePath];
	textureData.metadata = mipImages.GetMetadata();
	textureData.resourece = directXBase_->CreateTextureResource(textureData.metadata);
	textureData.intermediateResource = directXBase_->UploadTextureData(textureData.resourece.Get(), mipImages);
	//SRVの確保
	textureData.srvIndex = srvManager_->Allocate() + kSRVIndexTop;
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);
	//SRVの設定
	srvManager_->CreateSRVForTexture2D(textureData.srvIndex, textureData.resourece.Get(), textureData.metadata.format, UINT(textureData.metadata.mipLevels));
}

//テクスチャファイルのアンロード
void TextureManager::UnloadTexture(const std::string& filePath){
	auto it = textureDatas_.find(filePath);
	if (it != textureDatas_.end()) {
		SRVManager::GetInstance()->Free(it->second.srvIndex);//srvIndexの解放
		textureDatas_.erase(it);//削除
	} else {
		return;//存在しない場合何もしない
	}
}

// メタデータの取得
const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& filePath) {
	// TODO: return ステートメントをここに挿入します
	return textureDatas_[filePath].metadata;
}

// SRVインデックスの取得
uint32_t TextureManager::GetSRVIndex(const std::string& filePath) {
	return textureDatas_[filePath].srvIndex;
}

// GPUハンドルの取得
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSRVHandleGPU(const std::string& filePath) {
	return textureDatas_[filePath].srvHandleGPU;
}

//終了処理
void TextureManager::Finalize() {
	delete instance;
	instance = nullptr;
	isFinalize = true;
}
