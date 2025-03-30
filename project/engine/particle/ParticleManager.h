#pragma once
#include "engine/math/ResourceData.h"
#include <wrl.h>
#include <d3d12.h>
#include <memory>
#include <random>

//前方宣言
class DirectXBase;
class BaseObjectCommon;

/// <summary>
/// パーティクルの管理
/// </summary>
class ParticleManager {
private://エイリアステンプレート
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns>インスタンス</returns>
	static ParticleManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXBase">DirectXの基盤部分</param>
	void Initialize(DirectXBase* directXBase);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
private://メンバ関数
	//コンストラクタの封印
	ParticleManager() = default;
	//デストラクタの封印
	~ParticleManager() = default;
	//コピーコンストラクタ禁止
	ParticleManager(const ParticleManager&) = delete;
	//代入演算子の禁止
	ParticleManager operator=(const ParticleManager&) = delete;

	/// <summary>
	/// 頂点データの初期化
	/// </summary>
	void InitializeVertexData();

	/// <summary>
	/// 頂点リソースの生成
	/// </summary>
	void CreateVertexResorce();

	/// <summary>
	/// インデックスデータの初期化
	/// </summary>
	void InitializeIndexData();

	/// <summary>
	/// インデックスリソースの生成
	/// </summary>
	void CreateIndexResorce();
private://静的メンバ変数
	//インスタンス
	static inline ParticleManager* instance = nullptr;
	//Finalizeをしたかどうかのフラグ
	static inline bool isFinalize = false;
private://メンバ変数
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//オブジェクトの共通部分の基底クラス
	std::unique_ptr<BaseObjectCommon> baseObjectCommon_ = nullptr;
	//バッファリソース
	ComPtr<ID3D12Resource>vertexResource_ = nullptr;//頂点
	ComPtr<ID3D12Resource>indexResource_ = nullptr;//インデックス
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;//頂点
	uint32_t* indexData_ = nullptr;//インデックス
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};//頂点
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};//インデックス
	//ランダムエンジン
	std::mt19937 randomEngine_;
	std::uniform_real_distribution<float> randomDistribution(0.0f, 1.0f);
};

