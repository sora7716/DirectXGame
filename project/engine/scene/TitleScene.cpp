#include "TitleScene.h"
#include "engine/input/Input.h"
#include "SceneManager.h"
#include "SceneFactory.h"

//初期化
void TitleScene::Initialize(DirectXBase* directXBase) {
	//オーディオ
	audioManager_ = AudioManager::GetInstance();
	audioManager_->LoadAudio("Alarm01", "Alarm01.wav");
	audioManager_->FindAudio("Alarm01")->SoundPlayWave(true);
	//スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("monsterBall.png");
	object2d_ = std::make_unique<Object2d>();
	object2d_->Initialize();
	worldTransform_.scale = { 360.0f,360.0f };
	worldTransform_.translate = { 100.0f,100.0f };
	//シーンファクトリーの生成
	sceneFactory_ = new SceneFactory();
	//シーンファクトリーのセット
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
}

//更新
void TitleScene::Update() {
	//スプライト
	object2d_->SetSprite(sprite_.get());
	object2d_->SetScale(worldTransform_.scale);
	object2d_->SetTranslate(worldTransform_.translate);
	object2d_->Update();

	//Enterキーを押したら
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		//シーンをGameに切り替え
		SceneManager::GetInstance()->ChangeScene("Game");
	}
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
void TitleScene::Draw() {
	//スプライト
	object2d_->Draw();
}

//終了
void TitleScene::Finalize() {
	audioManager_->FindAudio("Alarm01")->StopAudio();
}
