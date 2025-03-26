#pragma once
#include "engine/2d/Sprite.h"
#include "engine/3d/Object3d.h"
#include "engine/debug/ImGuiManager.h"
#include "engine/audio/AudioManager.h"

//前方宣言
class SRVManager;
class DirectXBase;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene() = default;

	/// <summary>
    /// 初期化
    /// </summary>
    /// <param name="directXBase">DirectXの基盤部分</param>
	/// <param name="imguiManager">imGuiの管理</param>
	void Initialize(DirectXBase* directXBase,ImGuiManager*imguiManager);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

private://メンバ変数
	//スプライト
	std::unique_ptr<Sprite>sprite_ = nullptr;
	//スプライトの位置
	Transform2D worldTransform_ = {};
	//ImGuiの管理
	ImGuiManager* imguiManager_ = nullptr;
	//オーディオの管理
	AudioManager* audioManager_ = nullptr;
};

