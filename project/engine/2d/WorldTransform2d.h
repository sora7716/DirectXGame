#pragma once
#include "engine/math/ResourceData.h"
#include "engine/math/func/Rendering.h"
#include <string>
#include <wrl.h>
#include <d3d12.h>

//前方宣言
class DirectXBase;
class Camera;

/// <summary>
/// ワールドトランスフォーム(スプライト用)
/// </summary>
class WorldTransform2d {
private://エイリアステンプレート
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	WorldTransform2d() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WorldTransform2d() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXBase">DirectXの基盤部分</param>
	void Initialize(DirectXBase*directXBase);

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
    /// 座標変換行列リソースの生成
    /// </summary>
	void CreateTransformationMatrixResorce();

	/// <summary>
	/// ローカル座標の更新
	/// </summary>
	void UpdateTransform2d();
private://メンバ変数
	//ローカル座標
	Transform transform_ = {};
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//ワールドビュープロジェクションのリソース
	ComPtr<ID3D12Resource>wvpResource_ = nullptr;
	//ワールドビュープロジェクションのデータ
	TransformationMatrix* wvpData_ = nullptr;
public://メンバ変数
	//カメラ
	Camera* camera_ = nullptr;
	//ローカル座標
	Transform2D transform2d_ = {};
	//ワールド行列
	Matrix4x4 worldMatrix_ = {};
	//親
	const WorldTransform2d* parent_ = nullptr;
};

