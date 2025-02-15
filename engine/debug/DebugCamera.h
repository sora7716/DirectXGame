#pragma once
#include "engine/math/Vector3.h"
#include "engine/math/Matrix4x4.h"

/// <summary>
/// デバックカメラ
/// </summary>
class DebugCamera{
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DebugCamera() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DebugCamera() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
private://メンバ変数
	//X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = {0.0f,0.0f,0.0f};
	//ローカル座標
	Vector3 tranltation_ = { 0.0f,0.0f,-50.0f };
	//ビュー行列
	Matrix4x4 viewMatrix_ = {};
	//射影行列
	Matrix4x4 prijectionMatrix_ = {};
};

