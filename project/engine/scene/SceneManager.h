#pragma once
#include "IScene.h"

/// <summary>
/// シーン管理
/// </summary>
class SceneManager{
public://メンバ関数
private://メンバ変数
	//今のシーン(実行中のシーン)
	IScene *scene_ = nullptr;
};

