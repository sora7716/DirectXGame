#pragma once
#include "IScene.h"

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
	//オブジェクト2D
	std::unique_ptr<Object2d>object2d_[2] = { nullptr };
	//スプライトの位置
	Transform2D worldTransform_[2] = {};
	//オブジェクト3D
	std::unique_ptr<Object3d>object3d_[2] = { nullptr };
	Transform worldTransform3D_[2] = {};
	//カメラ
	Transform cameraWorldTransform_ = {};
	//オーディオの管理
	AudioManager* audioManager_ = nullptr;
	//シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;
};


