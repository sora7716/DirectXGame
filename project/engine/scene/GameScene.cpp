#include "GameScene.h"

//初期化
void GameScene::Initialize(DirectXBase* directXBase){
	//オーディオ
	audioManager_ = AudioManager::GetInstance();
	audioManager_->LoadAudio("mokugyo", "mokugyo.wav");
	audioManager_->FindAudio("mokugyo")->SoundPlayWave(true);
	//2Dオブジェクト
	object2d_ = std::make_unique<Object2d>();
	object2d_->Initialize();
	worldTransform_.scale = { 360.0f,360.0f };
	worldTransform_.translate = { 100.0f,100.0f };
}

//更新
void GameScene::Update(){
	//スプライト
	object2d_->SetSprite(SpriteManager::GetInstance()->FindSprite("block"));
	object2d_->SetScale(worldTransform_.scale);
	object2d_->SetTranslate(worldTransform_.translate);
	object2d_->Update();
	//ImGuiの受付開始
	ImGuiManager::GetInstance()->Begin();
#ifdef USE_IMGUI
	ImGui::Begin("sprite");
	ImGui::DragFloat2("scale", &worldTransform_.scale.x, 0.1f, 0.0f, 1000.0f, "%5.1f");
	ImGui::DragFloat2("translate", &worldTransform_.translate.x, 0.1f, 0.0f, 1000.0f, "%5.1f");
	ImGui::End();
#endif // USE_IMGUI
	//ImGuiの受付終了
	ImGuiManager::GetInstance()->End();
}

//描画
void GameScene::Draw(){
	//スプライト
	object2d_->Draw();
}

//終了
void GameScene::Finalize(){
}
