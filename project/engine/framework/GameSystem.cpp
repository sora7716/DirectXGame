#include "GameSystem.h"
#include "engine/scene/GameScene.h"
#include "engine/scene/TitleScene.h"

//初期化
void GameSystem::Initialize() {
	Framework::Initialize();
	//シーン
	scene_ = std::make_unique<TitleScene>();
	scene_->Initialize(directXBase_.get());
}

//更新
void GameSystem::Update() {
	Framework::Update();
	//シーン
	scene_->Update();
}

//描画
void GameSystem::Draw() {
	//描画開始位置
	directXBase_->PreDraw();
	//SRVの管理
	SRVManager::GetInstance()->PreDraw();
	//スプライトの共通部分
	SpriteCommon::GetInstance()->DrawSetting();
	//3Dオブジェクトの共通部分
	Object3dCommon::GetInstance()->DrawSetting();
	//シーン
	scene_->Draw();
	//ImGuiの管理
	ImGuiManager::GetInstance()->Draw();
	//描画終了位置
	directXBase_->PostDraw();
}

//終了
void GameSystem::Finalize() {
	Framework::Finalize();
	//シーン
	scene_->Finalize();
}
