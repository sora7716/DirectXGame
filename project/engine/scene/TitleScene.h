#pragma once
#include "IScene.h"
#include "engine/particle/ParticleEmit.h"

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
	std::unique_ptr<Object2d>object2d_ = nullptr;
	Transform2d worldTransform_ = {};
	Vector4 color = {};
	//3Dモデル
	std::unique_ptr<Object3d>object3d_ = nullptr;
	Transform worldTransform3d_ = {};
	Vector4 object3dColor_ = { 1.0f,1.0f,1.0f,1.0f };
	//平面
	std::unique_ptr<PlaneObject>plane_[2] = { nullptr };
	Transform2d planeTransform_[2] = {};
	//ライト
	DirectionalLight directionalLight_ = {};
	//ブレンドモード
	int blendMode_ = BlendMode::kNone;
	std::unique_ptr<ParticleEmit>particleEmit_ = nullptr;
};


