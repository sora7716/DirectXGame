#pragma once
#include "engine/3d/Model.h"
#include "engine/gameObject/WorldTransform.h"
#include <memory>

/// <summary>
/// パーティクルのエミット
/// </summary>
class ParticleEmit{
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParticleEmit() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParticleEmit() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXBase">DirectXの基盤部分</param>
	void Initialize(DirectXBase* directXBase);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
private://メンバ関数
	/// <summary>
	/// モデルデータの初期化
	/// </summary>
	void InitializeModelDate();

	/// <summary>
	/// 頂点リソースの生成
	/// </summary>
	void CreateVertexResource();

	/// <summary>
	/// インデックスリソースの生成
	/// </summary>
	void CreateIndexResource();

	/// <summary>
    /// マテリアルリソースの生成
    /// </summary>
	void CreateMaterialResource();
private://メンバ変数
	//モデルデータ
	ModelData modelData_ = {};
	//ModelCommonのポインタ
	ModelCommon* modelCommon_ = nullptr;
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//VertexResource
	ComPtr<ID3D12Resource>vertexResource_ = nullptr;
	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	//VertexResourceにデータを書き込むためのポインタ
	VertexData* vertexData_ = nullptr;
	//IndexResource
	ComPtr<ID3D12Resource>indexResource_ = nullptr;
	//IndexResourceにデータを書き込むためのポインタ
	uint32_t* indexData_ = nullptr;
	//IndexBufferView
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};//インデックス
	//ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
	//マテリアルリソース
	ComPtr<ID3D12Resource>materialResource_ = nullptr;
	//マテリアルリソースにデータを書き込むためのポインタ
	Material* materialData_ = nullptr;
};

