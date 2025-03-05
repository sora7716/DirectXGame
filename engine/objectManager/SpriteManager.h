#pragma once
#include "BaseObjectManager.h"
//前方宣言
class DirectXBase;
/// <summary>
/// スプライトの共通部分
/// </summary>
class SpriteManager :public BaseObjectManager{
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SpriteManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SpriteManager() = default;
};

