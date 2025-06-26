#include "Framework.h"
#include "engine/2d/TextureManager.h"
#include "engine/framework/GameObjectList.h"
#include "engine/math/func/Math.h"
#include "engine/audio/AudioManager.h"
#include "engine/input/Input.h"
#include "engine/3d/ModelManager.h"
#include "engine/camera/CameraManager.h"

//初期化
void Framework::Initialize() {
	//winApi
	WinApi::GetInstance()->Initialize();
	//DirectXの基盤部分
	directXBase_ = std::make_unique<DirectXBase>();
	directXBase_->Initialize();
	//SRVの管理
	SRVManager::GetInstance()->Initialize(directXBase_.get());
	//入力処理
	Input::GetInstance()->Initialize();
	//テクスチャ管理
	TextureManager::GetInstance()->Initialize(directXBase_.get());
	//モデルの管理
	ModelManager::GetInstance()->Initialize(directXBase_.get());
	//ImGuiの管理
	ImGuiManager::GetInstance()->Initialize(directXBase_.get());
	//スプライトの共通部分
	SpriteCommon::GetInstance()->Initialize(directXBase_.get());
	//2Dオブジェクトの共通部分
	Object2dCommon::GetInstance()->Initialize(directXBase_.get());
	//3Dオブジェクトの共通部分
	Object3dCommon::GetInstance()->Initialize(directXBase_.get());
	//パーティクルの共通部分
	ParticleCommon::GetInstance()->Initialize(directXBase_.get());
	//ゲームオブジェクトのリスト
	GameObjectList::GetInstance()->Initialize();
	//カメラの設定
	Object2dCommon::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->FindCamera("defaultCamera"));
	Object3dCommon::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->FindCamera("defaultCamera"));
	ParticleCommon::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->FindCamera("defaultCamera"));
	//シーンの管理
	SceneManager::GetInstance()->Initialize(directXBase_.get());
}

//更新
void Framework::Update() {
	//入力処理
	Input::GetInstance()->Update();
	//カメラの管理
	CameraManager::GetInstance()->Update();
	//シーンの管理
	SceneManager::GetInstance()->Update();
}

//終了
void Framework::Finalize() {
	//winApi
	WinApi::GetInstance()->Finalize();
	//DirectXの基盤部分
	directXBase_->Finalize();
	//SRVの管理
	SRVManager::GetInstance()->Finalize();
	//入力
	Input::GetInstance()->Finalize();
	//テクスチャ管理
	TextureManager::GetInstance()->Finalize();
	//モデルの管理
	ModelManager::GetInstance()->Finalize();
	//ImGuiの管理
	ImGuiManager::GetInstance()->Finalize();
	//カメラの管理
	CameraManager::GetInstance()->Finalize();
	//オーディオの管理
	AudioManager::GetInstance()->Finalize();
	//スプライトの共通部分
	SpriteCommon::GetInstance()->Finalize();
	//2Dオブジェクトの共通部分
	Object2dCommon::GetInstance()->Finalize();
	//3Dオブジェクトの共通部分
	Object3dCommon::GetInstance()->Finalize();
	//パーティクルの共通部分
	ParticleCommon::GetInstance()->Finalize();
	//シーンの管理
	SceneManager::GetInstance()->Finalize();
	//シーンファクトリーの解放
	delete sceneFactory_;
	//ゲームオブジェクトリスト
	GameObjectList::GetInstance()->Finalize();
}

//ゲームループ
void Framework::Run() {
	//ゲームシステムの初期化
	Initialize();
	//ウィンドウの✖ボタンが押されるまでループ
	while (isEndRequest()) {
		//ゲームシステムの更新
		Update();
		//ゲームシステムの描画
		Draw();
#ifdef _DEBUG
		//エスケイプを押したらループを抜ける
		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE) && Input::GetInstance()->PressKey(DIK_LSHIFT)) {
			break;
		}
#endif // _DEBUG
	}
	//ゲームシステムの終了
	Finalize();
}

//終了リクエスト
bool Framework::isEndRequest() {
	return WinApi::GetInstance()->ProcesMessage();
}
