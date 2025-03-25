#include "GameSystem.h"

//初期化
void GameSystem::Initialize() {
	Framework::Initialize();
	//オーディオ
	audioManager_->LoadAudio("mokugyo", "mokugyo.wav");
	audioManager_->FindAudio("mokugyo")->SoundPlayWave(true);
	//スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(spriteCommon_.get(),"block.png");
	worldTransform_.scale = { 360.0f,360.0f };
	worldTransform_.translate = { 100.0f,100.0f };
}

//更新
void GameSystem::Update() {
	Framework::Update();
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
void GameSystem::Draw() {
    //描画開始位置
	directXBase_->PreDraw();
    //SRVの管理
	srvManager_->PreDraw();
    //スプライトの共通部分
	spriteCommon_->DrawSetting();
    //3Dオブジェクトの共通部分
	object3dCommon_->DrawSetting();
	//スプライト
	sprite_->Draw();
	//ImGuiの管理
	imguiManager_->Draw();
	//描画終了位置
	directXBase_->PostDraw();
}

//終了
void GameSystem::Finalize() {
	Framework::Finalize();
}
