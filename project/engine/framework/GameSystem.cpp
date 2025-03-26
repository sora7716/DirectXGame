#include "GameSystem.h"

//初期化
void GameSystem::Initialize() {
	Framework::Initialize();
	//ゲームシーン
	gameScene_ = std::make_unique<GameScene>();
	gameScene_->Initialize(directXBase_.get(),imguiManager_.get());
}

//更新
void GameSystem::Update() {
	Framework::Update();
	//ゲームシーン
	gameScene_->Update();
}

//描画
void GameSystem::Draw() {
	//描画開始位置
	directXBase_->PreDraw();
	//SRVの管理
	srvManager_->PreDraw();
	//スプライトの共通部分
	SpriteCommon::GetInstance()->DrawSetting();
	//3Dオブジェクトの共通部分
	Object3dCommon::GetInstance()->DrawSetting();
	//ゲームシーン
	gameScene_->Draw();
	//ImGuiの管理
	imguiManager_->Draw();
	//描画終了位置
	directXBase_->PostDraw();
}

//終了
void GameSystem::Finalize() {
	Framework::Finalize();
	//ゲームシーン
	gameScene_->Finalize();
}
