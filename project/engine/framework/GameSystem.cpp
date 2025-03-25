#include "GameSystem.h"

//初期化
void GameSystem::Initialize() {
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
void GameSystem::Update() {
	//ImGuiの受付開始
	imguiManager_->Begin();
	//入力処理
	input_->Update();
	//カメラの管理
	cameraManager_->Update();
	//ImGuiの受付終了
	imguiManager_->End();
}

//描画
void GameSystem::Draw() {
    //描画開始位置
	directXBase_->PreDraw();
    //SRVの管理
	srvManager_->PreDraw();
    //ImGuiの管理
	imguiManager_->Draw();
    //スプライトの共通部分
	spriteCommon_->DrawSetting();
    //3Dオブジェクトの共通部分
	object3dCommon_->DrawSetting();
	//描画終了位置
	directXBase_->PostDraw();
}

//終了
void GameSystem::Finalize() {
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
