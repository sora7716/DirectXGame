#pragma once
#include "IScene.h"

/// <summary>
/// シーン管理
/// </summary>
class SceneManager {
public://メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXBase">DirectXの基盤部分</param>
	void Initialize(DirectXBase* directXBase);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 次のシーンのセッター
	/// </summary>
	/// <param name="nextScene">次のシーン</param>
	void SetNextScene(IScene* nextScene);
private://メンバ関数
private://メンバ変数
	//シーン
	IScene* scene_ = nullptr;
	//次のシーン
	IScene* nextScene_ = nullptr;
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
};

