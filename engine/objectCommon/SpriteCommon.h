#pragma once
#include "BaseObjectCommon.h"
//前方宣言
class DirectXBase;
/// <summary>
/// スプライトの共通部分
/// </summary>
class SpriteCommon :public BaseObjectCommon{
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SpriteCommon() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SpriteCommon() = default;
};

