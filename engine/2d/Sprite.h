#pragma once

//前方宣言
class SpriteGeneral;
class DirectXBase;

/// <summary>
/// スプライト
/// </summary>
class Sprite {
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(SpriteGeneral* spriteGeneral);
private://メンバ変数
	SpriteGeneral* spriteGeneral_ = nullptr;//スプライトの共通部分
	DirectXBase* directXBase_ = nullptr;//DirectXの基盤
};

