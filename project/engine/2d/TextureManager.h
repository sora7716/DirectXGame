#pragma once
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"
#include <string>
#include <unordered_map>

//前方宣言
class DirectXBase;
class SRVManager;

/// <summary>
/// テクスチャを管理する
/// </summary>
class TextureManager {
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
private://構造体
	//テクスチャデータ
	typedef struct TextureData {
		DirectX::TexMetadata metadata;//画像の幅や高さなどの情報
		ComPtr<ID3D12Resource>resourece;//テクスチャリソース
		uint32_t srvIndex;//SRVインデックス
		ComPtr<ID3D12Resource>intermediateResource;//アップロードするリソース
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
	void Initialize(DirectXBase* directXBase);

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	/// <param name="filePath">テクスチャのファイルパス</param>
	void LoadTexture(const std::string& filePath);

	/// <summary>
	/// テクスチャファイルのアンロード
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void UnloadTexture(const std::string& filePath);

	/// <summary>
	/// メタデータの取得
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>メタデータ</returns>
	const DirectX::TexMetadata& GetMetaData(const std::string& filePath);

	/// <summary>
	/// SRVインデックスの取得
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>SRVインデックス</returns>
	uint32_t GetSRVIndex(const std::string& filePath);

	/// <summary>
	/// GPUハンドルの取得
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>GPUハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleGPU(const std::string& filePath);

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
	//Finalizeしたかのフラグ
	static inline bool isFinalize = false;
private://メンバ変数
	//テクスチャデータ
	std::unordered_map<std::string, TextureData>textureDatas_;
	//DirectX基盤
	DirectXBase* directXBase_ = nullptr;
	//SRVの管理
	SRVManager* srvManager_ = nullptr;
};