#pragma once
#include <stdint.h>
#include <wrl.h>
#include <d3d12.h>
#include <queue>

//前方宣言
class DirectXBase;

/// <summary>
/// SRV管理
/// </summary>
class SRVManager {
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns>インスタンス</returns>
	static SRVManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXBase">DirectXBaseの基盤部分</param>
	void Initialize(DirectXBase* directXBase);

	/// <summary>
	/// 確保
	/// </summary>
	/// <returns>インデックス</returns>
	uint32_t Allocate();

	/// <summary>
	/// 解放
	/// </summary>
	/// <param name="index">インデックス</param>
	void Free(uint32_t index);

	/// <summary>
	/// SRV生成(テクスチャ用)
	/// </summary>
	/// <param name="srvIndex">srvインデックス</param>
	/// <param name="resource">リソース</param>
	/// <param name="format">フォーマット</param>
	/// <param name="mipLevels">ミップレベル</param>
	void CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* resource, DXGI_FORMAT format, UINT mipLevels);

	/// <summary>
	/// SRV生成(Structured Buffer用)
	/// </summary>
	/// <param name="srvIndex">srvインデックス</param>
	/// <param name="resource">リソース</param>
	/// <param name="numElements">要素数</param>
	/// <param name="structureByteStride"></param>
	void CreateSRVForStructuredBuffer(uint32_t srvIndex, ID3D12Resource* resource, UINT numElements, UINT structureByteStride);

	/// <summary>
	/// 描画開始位置
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// rootDescriptorTableのセッター
	/// </summary>
	/// <param name="rootParameterIndex">rootParameterのインデックス</param>
	/// <param name="srvIndex">srvインデックス</param>
	void SetGrahicsRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex);

	/// <summary>
	/// 最大テクスチャを超えて読み込もうとしてるかチェック
	/// </summary>
	/// <param name="kSRVTop">SRVの最初の値</param>
	/// <returns>最大テクスチャを超えて読み込もうとしてるか</returns>
	bool TextureLimitCheck(uint32_t kSRVTop);

	/// <summary>
	/// CPUデスクリプタハンドルのゲッター
	/// </summary>
	/// <param name="index">インデックス</param>
	/// <returns>CPUデスクリプタハンドル</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// GPUデスクリプタハンドルのゲッター
	/// </summary>
	/// <param name="index">インデックス</param>
	/// <returns>GPUデスクリプタハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// デスクリプタヒープのゲッター
	/// </summary>
	/// <returns>デスクリプタヒープ</returns>
	ID3D12DescriptorHeap* GetDescriptorHeap()const;
private://メンバ関数
	//コンストラクタの封印
	SRVManager() = default;
	//デストラクタの封印
	~SRVManager() = default;
	//コピーコンストラクタ禁止
	SRVManager(const SRVManager&) = delete;
	//代入演算子の禁止
	SRVManager operator=(const SRVManager&) = delete;
public://定数
	static inline const uint32_t kMaxSRVCount = 512;
private://静的メンバ変数
	//インスタンス
	static inline SRVManager* instance = nullptr;
	//Finalizeをしたかどうか
	static inline bool isFinalize = false;
private://メンバ変数
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//SRV用のデスクリプタサイズ
	uint32_t descriptorSize_ = 0;
	//SRV用のデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>descriptorHeap_ = nullptr;
	//次に使用するSRVインデックス
	uint32_t useIndex_ = 0;
	//空きリスト
	std::queue<uint32_t>freeList_;
};

