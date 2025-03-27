#pragma once
#include "AbstractSceneFactory.h"

/// <summary>
/// このゲーム用のシーン工場
/// </summary>
class SceneFactory :public AbstractSceneFactory {
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneFactory() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneFactory() = default;

	/// <summary>
	/// シーンの生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns>シーン</returns>
	IScene* CreateScene(const std::string& sceneName)override;
};

