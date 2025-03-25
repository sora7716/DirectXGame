#include "Framework.h"

//初期化
void Framework::Initialize(){
	//winApi
	winApi_ = WinApi::GetInstance();
	winApi_->Initialize();
	//DirectXの基盤部分
	directXBase_ = std::make_unique<DirectXBase>();
	directXBase_->Initialize();
	//SRVの管理
	srvManager_ = std::make_unique<SRVManager>();
	srvManager_->Initialize(directXBase_.get());
	//入力処理
	input_ = Input::GetInstance();
	input_->Initialize();
	//テクスチャ管理
	textureManager_ = TextureManager::GetInstance();
	textureManager_->Initialize(directXBase_.get(), srvManager_.get());
	//モデルの管理
	modelManager_ = ModelManager::GetInstance();
	modelManager_->Initialize(directXBase_.get());
	//ImGuiの管理
	imguiManager_ = std::make_unique<ImGuiManager>();
	imguiManager_->Initialize(winApi_, directXBase_.get(), srvManager_.get());
	//スプライトの共通部分
	spriteCommon_ = std::make_unique<SpriteCommon>();
	spriteCommon_->Initialize(directXBase_.get());
	//3Dオブジェクトの共通部分
	object3dCommon_ = std::make_unique<Object3dCommon>();
	object3dCommon_->Initialize(directXBase_.get());
	//カメラの管理
	cameraManager_ = CameraManager::GetInstance();
	//オーディオの管理
	audioManager_ = AudioManager::GetInstance();
}

//更新
void Framework::Update(){
	//入力処理
	input_->Update();
	//カメラの管理
	cameraManager_->Update();
}

//終了
void Framework::Finalize(){
	//winApi
	winApi_->Finalize();
	//DirectXの基盤部分
	directXBase_->Finalize();
	//入力
	input_->Finalize();
	//テクスチャ管理
	textureManager_->Finalize();
	//モデルの管理
	modelManager_->Finalize();
	//ImGuiの管理
	imguiManager_->Finalize();
	//カメラの管理
	cameraManager_->Finalize();
	//オーディオの管理
	audioManager_->Finalize();
}

//ゲームループ
void Framework::Run(){
	//ゲームシステムの初期化
	Initialize();
	//ウィンドウの✖ボタンが押されるまでループ
	while (true) {
		if (isEndRequest()) {
			break;
		}
		//ゲームシステムの更新
		Update();
		//ゲームシステムの描画
		Draw();
	}
	//ゲームシステムの終了
	Finalize();
}

//終了リクエスト
bool Framework::isEndRequest(){
	return WinApi::GetInstance()->ProcesMessage();
}
