#pragma once
#include "IScene.h"
#include "engine/2d/Sprite.h"
#include "engine/particle/ParticleSystem.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene :public IScene {
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene()override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXBase">DirectXの基盤部分</param>
	void Initialize(DirectXBase* directXBase)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize()override;

private://メンバ変数
	//2Dモデル
	std::unique_ptr <Object2d> object2d_ = nullptr;
	Transform2d worldTransform2d_ = {};
	Vector4 color = {};
	//3Dモデル
	std::unique_ptr<Object3d>object3d_ = nullptr;
	Transform worldTransform3d_ = {};
	Vector4 object3dColor_ = { 1.0f,1.0f,1.0f,1.0f };
	//ライト
	DirectionalLight directionalLight_ = {};

	std::unique_ptr<Sprite>sprite_ = nullptr;

	std::unique_ptr<ParticleSystem>particleSystem_ = nullptr;

	//ブレンドモード
	int blendMode_ = BlendMode::kNone;
};


