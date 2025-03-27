#pragma once
#include <random>
#include <wrl.h>
#include <d3d12.h>
#include <list>
#include <unordered_map>
#include <string>
#include "Particle.h"
#include "engine/math/ResourceData.h"
#include "engine/math/func/Rendering.h"
#include "engine/3d/Model.h"
//前方宣言
class DirectXBase;
class SRVManager;
class BaseObjectCommon;
class Camera;

//インスタンシングデータ
typedef struct InstanceData {
	Transform transform;
	Vector4 color;
}InstanceData;

//パーティクルグループ
typedef struct ParticleGroup {
	MaterialData materialData;
	std::list<Particle>particleList;
	uint32_t srvIndex;
	Microsoft::WRL::ComPtr<ID3D12Resource>resource;
	uint32_t instanceCount;
	InstanceData* instancePtr;
}ParticleGroup;
/// <summary>
/// パーティクルの管理
/// </summary>
class ParticleManager {
private://エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr <T>;
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
	/// <param name="baseObjectCommon">オブジェクトの共通部分</param>
	void Initialize(DirectXBase* directXBase, BaseObjectCommon* baseObjectCommon);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Update(Camera*camera);

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
	//代入演算子禁止
	ParticleManager operator=(const ParticleManager&) = delete;

	/// <summary>
	/// 頂点データの初期化
	/// </summary>
	void InitializeVertexData();

	/// <summary>
	/// インデックスデータの初期化
	/// </summary>
	void InitializeIndexData();

	/// <summary>
	/// 頂点リソースの生成
	/// </summary>
	void CreateVertexResource();

	/// <summary>
	/// インデックスリソースの生成
	/// </summary>
	void CreateIndexResource();

	/// <summary>
	/// パーティクルグループの生成
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="textureFilePath">テクスチャ</param>
	void CreateParticleGroup(const std::string& name, const std::string& textureFilePath);

private://静的メンバ変数
	//インスタンス
	static inline ParticleManager* instance = nullptr;
	//Finalize()を読んだかどうか
	static inline bool isFinalize = false;
private://メンバ変数
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//オブジェクトの共通部分
	BaseObjectCommon* baseObjectCommon_ = nullptr;
	//パイプラインステート
	ComPtr<ID3D12PipelineState>graphicsPiplene_ = nullptr;
	//頂点データ
	VertexData* vertexData_ = nullptr;
	//インデックスデータ
	uint32_t* indexData_ = nullptr;
	//頂点リソース
	ComPtr<ID3D12Resource>vertexResource_ = nullptr;
	//インデックスリソース
	ComPtr<ID3D12Resource>indexResource_ = nullptr;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};//頂点
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};//インデックス
	//パーティクルグループコンテナ
	std::unordered_map<std::string, ParticleGroup>particleGroups_;
	//ランダムエンジン
	std::mt19937 randomEngine_;
	//ランダムデバイス
	std::random_device randomDevice_;
};

