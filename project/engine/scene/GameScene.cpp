#include "GameScene.h"

//初期化
void GameScene::Initialize(DirectXBase* directXBase){
	
}

//更新
void GameScene::Update(){
	
	//ImGuiの受付開始
	ImGuiManager::GetInstance()->Begin();
#ifdef USE_IMGUI
	
#endif // USE_IMGUI
	//ImGuiの受付終了
	ImGuiManager::GetInstance()->End();
}

//描画
void GameScene::Draw(){
	
}

//終了
void GameScene::Finalize(){
}
