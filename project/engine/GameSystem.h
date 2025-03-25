#pragma once
#include <memory>
//前方宣言
class WinApi;
class DirectXBase;
class Input;
class SRVManager;
class ImGuiManager;
class AudioManager;
class Object3dCommon;
class SpriteCommon;
class CameraManager;
class TextureManager;
class ModelManager;

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

