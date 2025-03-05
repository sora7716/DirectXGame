#pragma once
#include "BaseObjectManager.h"
/// <summary>
/// 3Dオブジェクトの共通部分
/// </summary>
class Object3dManager:public BaseObjectManager{
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3dManager() = default;
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Object3dManager() = default;
};

