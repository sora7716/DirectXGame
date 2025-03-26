#include "GameScene.h"

//初期化
void GameScene::Initialize(DirectXBase* directXBase, ImGuiManager* imguiManager){
	//imGuiの管理
	imguiManager_ = imguiManager;
	//オーディオ
	audioManager_ = AudioManager::GetInstance();
	audioManager_->LoadAudio("mokugyo", "mokugyo.wav");
	audioManager_->FindAudio("mokugyo")->SoundPlayWave(true);
	//スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("block.png");
	worldTransform_.scale = { 360.0f,360.0f };
	worldTransform_.translate = { 100.0f,100.0f };
}

//更新
void GameScene::Update(){
	//スプライト
	sprite_->SetScale(worldTransform_.scale);
	sprite_->SetTranslate(worldTransform_.translate);
	sprite_->Update();
	//ImGuiの受付開始
	imguiManager_->Begin();
#ifdef USE_IMGUI
	ImGui::Begin("sprite");
	ImGui::DragFloat2("scale", &worldTransform_.scale.x, 0.1f, 0.0f, 1000.0f, "%5.1f");
	ImGui::DragFloat2("translate", &worldTransform_.translate.x, 0.1f, 0.0f, 1000.0f, "%5.1f");
	ImGui::End();
#endif // USE_IMGUI
	//ImGuiの受付終了
	imguiManager_->End();
}

//描画
void GameScene::Draw(){
	//スプライト
	sprite_->Draw();
}

//終了
void GameScene::Finalize(){
}
