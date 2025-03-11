#pragma once
#include "BaseObjectCommon.h"
//前方宣言
class Camera;

/// <summary>
/// 3Dオブジェクトの共通部分
/// </summary>
class Object3dCommon :public BaseObjectCommon {
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3dCommon() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Object3dCommon() = default;

	/// <summary>
	/// デフォルトカメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetDefaultCamera(Camera* camera);

	/// <summary>
	/// デフォルトカメラのゲッター
	/// </summary>
	/// <returns>デフォルトカメラ</returns>
	Camera* GetDefaultCamera()const;
private://メンバ変数
	Camera* defaultCamera_ = nullptr;
};

