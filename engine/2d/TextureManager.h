#pragma once
#include "engine/base/DirectXBase.h"
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"

/// <summary>
/// テクスチャを管理する
/// </summary>
class TextureManager {
private://構造体
	//テクスチャデータ
	typedef struct TextureData {
		std::string filePath;//画像ファイルのパス
		DirectX::TexMetadata metadata;//画像の幅や高さなどの情報
		DirectXBase::ComPtr<ID3D12Resource>resourece;//テクスチャリソース
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;//SRV作成時に必要なCPUハンドル
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;//描画コマンドに必要なGPUハンドル
	}TextureData;
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns>インスタンス</returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXBase">DirectXの基盤</param>
	void Initialize(DirectXBase*directXBase);

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	/// <param name="filePath">テクスチャのファイルパス</param>
	void LoadTexture(const std::string& filePath);

	/// <summary>
	/// SRVインデックスの開始番号
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>テクスチャ番号</returns>
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);

	/// <summary>
	/// テクスチャ番号からGPUハンドルを取得
	/// </summary>
	/// <param name="textureIndex">テクスチャ番号</param>
	/// <returns>GPUハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();
private://メンバ関数
	//コンストラクタの封印
	TextureManager() = default;
	//デストラクタの封印
	~TextureManager() = default;
	//コピーコンストラクタ禁止
	TextureManager(TextureManager&) = delete;
	//代入演算子の禁止
	TextureManager& operator=(TextureManager&) = delete;
private://静的メンバ変数
	//TextureManagerのインスタンス
	static TextureManager* instance;
	//SRVインデックスの開始番号
	static uint32_t kSRVIndexTop;
private://メンバ変数
	//テクスチャデータ
	std::vector<TextureData> textureDatas_;
	//DirectX基盤
	DirectXBase* directXBase_ = nullptr;
};