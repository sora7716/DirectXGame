#pragma once
#include "WinApi.h"
#include "engine/base/DirectXBase.h"
#include "engine/math/func/Math.h"
#include "engine/audio/AudioManager.h"
#include "engine/input/Input.h"
#include "engine/objectCommon/SpriteCommon.h"
#include "engine/objectCommon/Object2dCommon.h"
#include "engine/objectCommon/Object3dCommon.h"
#include "engine/3d/ModelManager.h"
#include "engine/3d/CameraManager.h"
#include "engine/base/SRVManager.h"
#include"engine/debug/ImGuiManager.h"
#include "engine/scene/SceneManager.h"
#include "engine/scene/AbstractSceneFactory.h"
#include "engine/2d/SpriteManager.h"

/// <summary>
/// ゲーム全体
/// </summary>
class Framework {
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Framework() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Framework() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// ゲームループ
	/// </summary>
	void Run();

	/// <summary>
	/// 終了リクエスト
	/// </summary>
	/// <returns>終了したかどうか</returns>
	virtual bool isEndRequest();
protected://メンバ変数
	//入力処理
	Input* input_ = nullptr;
	//カメラの管理
	CameraManager* cameraManager_ = nullptr;
	//オーディオの管理
	AudioManager* audioManager_ = nullptr;
	//DirectXの基盤部分
	std::unique_ptr<DirectXBase>directXBase_ = nullptr;
	//シーンファクトリー
	AbstractSceneFactory*sceneFactory_=nullptr;
};

