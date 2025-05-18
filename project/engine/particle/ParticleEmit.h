#pragma once
#include "engine/math/ResourceData.h"
#include "engine/math/RenderingData.h"
#include "engine/blend/BlendMode.h"
#include <d3d12.h>
#include <wrl.h>
#include <array>
#include <memory>
//前方宣言
class DirectXBase;
class Camera;

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

	/// <summary>
	/// ワールドトランスフォームのリソースの生成
	/// </summary>
	void CreateWorldTransformResource();

	/// <summary>
	/// ワールドトランスフォームの更新
	/// </summary>
	void UpdateWorldTransform();

	/// <summary>
	/// ストラクチャバッファの生成
	/// </summary>
	void CreateStructuredBuffer();
private://静的メンバ変数
	//パーティクルの数
	static const uint32_t kNumInstanceCount = 10;
private://メンバ変数
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//カメラ
	Camera* camera_ = nullptr;
	//ワールドビュープロジェクションのリソース
	ComPtr<ID3D12Resource>wvpResource_ = nullptr;
	//ワールドビュープロジェクションのデータ
	std::array<TransformationMatrix, kNumInstanceCount> instanceDatas_ = {};
	//トランスフォーム
	Transform transform_ = {};
	//ワールドマトリックス
	Matrix4x4 worldMatrix_ = {};
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

	//ストラクチャバッファの生成に使用
	// ストラクチャバッファのリソース
	ComPtr<ID3D12Resource>instancingResource_ = nullptr;
	//SRVインデックス
	uint32_t srvIndex_ = 0;
};
