#pragma once
#include "engine/2d/Object2d.h"
#include "engine/3d/Object3d.h"
#include "engine/debug/ImGuiManager.h"
#include "engine/audio/AudioManager.h"
#include "engine/camera/CameraManager.h"
#include "engine/base/DirectXBase.h"
#include "engine/base/SRVManager.h"
#include "engine/blend/BlendMode.h"

//前方宣言
class SceneManager;
class AbstractSceneFactory;

/// <summary>
/// シーンのインターフェース
/// </summary>
class IScene {
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	IScene() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IScene() = default;
	//純粋仮想関数
	virtual void Initialize(DirectXBase*) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
};

