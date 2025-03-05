#pragma once
#include "BaseObjectManager.h"
/// <summary>
/// 3Dオブジェクトの共通部分
/// </summary>
class Object3DManager:public BaseObjectManager{
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3DManager() = default;
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Object3DManager() = default;
};

