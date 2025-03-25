#pragma once
#include "WinApi.h"
#include "base/DirectXBase.h"
#include "2d/TextureManager.h"
#include "math/func/Math.h"
#include "audio/AudioManager.h"
#include "input/Input.h"
#include "objectCommon/SpriteCommon.h"
#include "2d/Sprite.h"
#include "objectCommon/Object3dCommon.h"
#include "3d/Object3d.h"
#include "3d/ModelManager.h"
#include "3d/CameraManager.h"
#include "base/SRVManager.h"
#include"debug/ImGuiManager.h"

/// <summary>
/// ゲーム全体
/// </summary>
class GameSystem{
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameSystem()=default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameSystem()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

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
	//ウィンドウズApi
	WinApi* winApi_ = nullptr;
	//入力処理
	Input* input_ = nullptr;
	//テクスチャの管理
	TextureManager* textureManager_ = nullptr;
	//カメラの管理
	CameraManager* cameraManager_ = nullptr;
	//オーディオの管理
	AudioManager* audioManager_ = nullptr;
	//モデルの管理
	ModelManager* modelManager_ = nullptr;
	//DirectXの基盤部分
	std::unique_ptr<DirectXBase>directXBase_ = nullptr;
	//SRVの管理
	std::unique_ptr<SRVManager>srvManager_ = nullptr;
	//ImGuiの管理
	std::unique_ptr<ImGuiManager>imguiManager_ = nullptr;
	//スプライトの共通部分
	std::unique_ptr<SpriteCommon>spriteCommon_ = nullptr;
	//3Dオブジェクトの共通部分
	std::unique_ptr<Object3dCommon>object3dCommon_ = nullptr;
};

