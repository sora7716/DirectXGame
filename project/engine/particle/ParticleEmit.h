
#pragma once
#include "engine/math/ResourceData.h"
#include "engine/gameObject/WorldTransform.h"
#include "engine/blend/BlendMode.h"
#include <d3d12.h>
#include <wrl.h>
#include <memory>

/// <summary>
/// パーティクルのエミット
/// </summary>
class ParticleEmit {
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

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
private://メンバ関数
	/// <summary>
	/// モデルデータの初期化
	/// </summary>
	void InitializeModelData();

	/// <summary>
	/// マテリアルデータの初期化
	/// </summary>
	void InitializeMaterialData();

	/// <summary>
	/// 頂点リソースの生成
	/// </summary>
	void CreateVertexResource();

	/// <summary>
	/// マテリアルリソースの生成
	/// </summary>
	void CreateMaterialResource();
private://メンバ変数
	//パーティクルの数
	uint32_t instanceCoount_ = 10;
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
	//トランスフォーム
	Transform transform_ = {};
	//モデルデータ
	ModelData modelData_ = {};
	//マテリアルデータ
	Material* materialData_ = nullptr;
	//バッファリソース
	ComPtr<ID3D12Resource>vertexResource_ = nullptr;//頂点
	ComPtr<ID3D12Resource>materialResource_ = nullptr;//マテリアル
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};//頂点
	//ブレンドモード
	BlendMode blendMode_ = BlendMode::kNone;
};
