#include "GameSystem.h"
#include "engine/scene/SceneManager.h"
#include "engine/scene/SceneFactory.h"
//初期化
void GameSystem::Initialize() {
	Framework::Initialize();
	//シーンファクトリーの生成
	sceneFactory_ = new SceneFactory();
	//シーンファクトリーのセット
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	//タイトルシーンを呼び出す
	SceneManager::GetInstance()->ChangeScene("Title");
}

//更新
void GameSystem::Update() {
	Framework::Update();
}

//描画
void GameSystem::Draw() {
	//描画開始位置
	directXBase_->PreDraw();
	//SRVの管理
	SRVManager::GetInstance()->PreDraw();
	//シーン
	SceneManager::GetInstance()->Draw();
	//ImGuiの管理
	ImGuiManager::GetInstance()->Draw();
	//描画終了位置
	directXBase_->PostDraw();
}

//終了
void GameSystem::Finalize() {
	Framework::Finalize();
}
