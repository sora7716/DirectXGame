#include "TitleScene.h"
#include "engine/input/Input.h"
#include "SceneManager.h"
#include "SceneFactory.h"
#include "engine/3d/ModelManager.h"

//初期化
void TitleScene::Initialize(DirectXBase* directXBase) {
	//オーディオ
	audioManager_ = AudioManager::GetInstance();
	audioManager_->LoadAudio("Alarm01", "Alarm01.wav");
	audioManager_->FindAudio("Alarm01")->SoundPlayWave(true);
	//スプライト
	for (int i = 0;i < 2;i++) {
		sprite_[i] = std::make_unique<Sprite>();
		object2d_[i] = std::make_unique<Object2d>();
		object2d_[i]->Initialize();
	}
	sprite_[0]->Initialize("engine/resources/texture/monsterBall.png");
	sprite_[1]->Initialize("engine/resources/texture/block.png");
	worldTransform_[0].scale = { 360.0f,360.0f };
	worldTransform_[0].rotate = 0.0f;
	worldTransform_[0].translate = { 100.0f,100.0f };
	worldTransform_[1].scale = { 1.0f,1.0f };
	worldTransform_[1].rotate = 0.0f;
	worldTransform_[1].translate = { 1.4f,0.0f };
	//3Dオブジェクト
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	//モデルの読み込み
	ModelManager::GetInstance()->LoadOBJModel("base","axis");
	object3d_->SetModel("axis");
	//シーンファクトリーの生成
	sceneFactory_ = new SceneFactory();
	//シーンファクトリーのセット
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
}

//更新
void TitleScene::Update() {
	//スプライト
	object2d_[0]->SetSprite(sprite_[0].get());
	object2d_[1]->SetSprite(sprite_[1].get());
	for (int i = 0;i < 2;i++) {
		object2d_[i]->SetScale(worldTransform_[i].scale);
		object2d_[i]->SetRotate(worldTransform_[i].rotate);
		object2d_[i]->SetTranslate(worldTransform_[i].translate);
		object2d_[i]->Update();
	}
	object2d_[1]->SetParent(&object2d_[0]->GetWorldMatrix());
	//Enterキーを押したら
	if (Input::GetInstance()->TriggerKey(DIK_LALT)) {
		//シーンをGameに切り替え
		SceneManager::GetInstance()->ChangeScene("Game");
		audioManager_->FindAudio("Alarm01")->StopAudio();
	}
	//3Dオブジェクト
	object3d_->SetRotate(worldTransform3D_.rotate);
	object3d_->Update();
	//ImGuiの受付開始
	ImGuiManager::GetInstance()->Begin();
#ifdef USE_IMGUI
	ImGui::Begin("sprite");
	ImGui::DragFloat2("scale[0]", &worldTransform_[0].scale.x, 0.1f, 0.0f, 1000.0f, "%5.1f");
	ImGui::DragFloat("rotate[0]", &worldTransform_[0].rotate, 0.1f, 0.0f, 1000.0f, "%5.1f");
	ImGui::DragFloat2("translate[0]", &worldTransform_[0].translate.x, 0.1f, 0.0f, 1000.0f, "%5.1f");
	ImGui::DragFloat2("scale[1]", &worldTransform_[1].scale.x, 0.1f, 0.0f, 1000.0f, "%5.1f");
	ImGui::DragFloat("rotate[1]", &worldTransform_[1].rotate, 0.1f, 0.0f, 1000.0f, "%5.1f");
	ImGui::DragFloat2("translate[1]", &worldTransform_[1].translate.x, 0.1f, 0.0f, 1000.0f, "%5.1f");
	ImGui::End();

	ImGui::Begin("3dObject");
	ImGui::DragFloat3("rotate", &worldTransform3D_.rotate.x, 0.1f);
	ImGui::End();
#endif // USE_IMGUI
	//ImGuiの受付終了
	ImGuiManager::GetInstance()->End();
}

//描画
void TitleScene::Draw() {
	//スプライト
	/*for (int i = 0;i < 2;i++) {
		object2d_[i]->Draw();
	}*/
	//3Dオブジェクト
	object3d_->Draw();
}

//終了
void TitleScene::Finalize() {
}
