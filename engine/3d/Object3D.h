#pragma once
#include "engine/math/ResourceData.h"
#include "engine/math/rendering/Rendering.h"
#include <vector>
#include <string>
#include <wrl.h>
#include <d3d12.h>
#include "Model.h"

//前方宣言
class Object3dCommon;

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d{
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3d() = default;
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Object3d() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="object3dCommon">3Dオブジェクトの共通部分</param>
	void Initialize(Object3dCommon* object3dCommon);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// モデルのセッター
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* model);
private://メンバ関数
	/// <summary>
	/// 座標変換行列データの生成
	/// </summary>
	void CreateTransformationMatrixData();

	/// <summary>
	/// 光源の生成
	/// </summary>
	void CreateDirectionLight();
private://メンバ変数
	//ローカル座標
	Transform transform_ = {};
	// カメラ座標
	Transform cameraTransform_ = {};
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//3Dオブジェクトの共通部分
	Object3dCommon* object3dCommon_ = nullptr;	
	//3Dモデル
	Model* model_ = nullptr;
	//バッファリソース
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;//座標変換行列
	ComPtr<ID3D12Resource> directionalLightResource_=nullptr;//光源
	//バッファリソース内のデータを指すポインタ
	TransformationMatrix* wvpData_ = nullptr;//座標変換行列
	DirectionalLight* directionalLightData_ = nullptr;//光源
};

