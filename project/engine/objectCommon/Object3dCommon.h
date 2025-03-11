#pragma once
#include "BaseObjectCommon.h"
/// <summary>
/// 3Dオブジェクトの共通部分
/// </summary>
class Object3dCommon:public BaseObjectCommon{
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
};

