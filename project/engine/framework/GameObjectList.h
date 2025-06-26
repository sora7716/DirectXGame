#pragma once
#include "engine/audio/AudioManager.h"
#include "engine/3d/ModelManager.h"
#include "engine/camera/CameraManager.h"

/// <summary>
/// ゲームで使用するオブジェクトの読み込みのリスト
/// </summary>
class GameObjectList{
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns>インスタンス</returns>
	static GameObjectList* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
private://メンバ関数

	//コンストラクタの封印
	GameObjectList() = default;
	//デストラクタの封印
	~GameObjectList() = default;
	//コピーコンストラクタ禁止
	GameObjectList(const GameObjectList&) = delete;
	//代入演算子の禁止
	GameObjectList operator=(const GameObjectList&) = delete;

	/// <summary>
	/// オーディオの読み込み
	/// </summary>
	void LoadAudio();

	/// <summary>
	/// スプライトの読み込み
	/// </summary>
	void LoadSprite();

	/// <summary>
	/// OBJファイルの読み込み
	/// </summary>
	void LoadModel();

	/// <summary>
	/// カメラの生成
	/// </summary>
	void CreateCamera();
private://静的メンバ変数
	//インスタンス
	static inline GameObjectList* instance = nullptr;
	//Finalizeをしたかどうか
	static inline bool isFinalize = false;
private://メンバ変数
	//オーディオの管理
	AudioManager* audioManager_ = nullptr;
	//モデルの管理
	ModelManager* modelManager_ = nullptr;
	//カメラの管理
	CameraManager* cameraManager_ = nullptr;
};

