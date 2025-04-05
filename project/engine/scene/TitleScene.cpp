#include "TitleScene.h"
#include "engine/input/Input.h"
#include "SceneManager.h"
#include "SceneFactory.h"
#include "engine/3d/ModelManager.h"

//初期化
void TitleScene::Initialize(DirectXBase* directXBase) {
	//オーディオ
	AudioManager::GetInstance()->FindAudio("Alarm01")->SoundPlayWave(true);
	//2Dオブジェクト
	for (int i = 0;i < 2;i++) {
		object2d_[i] = std::make_unique<Object2d>();
		object2d_[i]->Initialize();
	}
	worldTransform_[0].scale = { 360.0f,360.0f };
	worldTransform_[0].rotate = 0.0f;
	worldTransform_[0].translate = { 100.0f,100.0f };
	worldTransform_[1].scale = { 1.0f,1.0f };
	worldTransform_[1].rotate = 0.0f;
	worldTransform_[1].translate = { 1.4f,0.0f };
	
	//3Dオブジェクト
	for (int i = 0;i < 2;i++) {
		object3d_[i] = std::make_unique<Object3d>();
		object3d_[i]->Initialize();
		object3d_[i]->SetModel("ziku");
	}
	worldTransform3D_[1].translate = { 5.0f,0.0f,0.0f };
	//カメラ
	cameraWorldTransform_.translate = { 0.0f,0.0f,-10.0f };
	//シーンファクトリーの生成
	sceneFactory_ = new SceneFactory();
	//シーンファクトリーのセット
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	//スプライト
	object2d_[0]->SetSprite(SpriteManager::GetInstance()->FindSprite("pokemon"));
	object2d_[1]->SetSprite(SpriteManager::GetInstance()->FindSprite("enemy"));
}

//更新
void TitleScene::Update() {
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
		AudioManager::GetInstance()->FindAudio("Alarm01")->StopAudio();
	}
	//カメラ
	CameraManager::GetInstance()->FindCamera("titleCamera")->SetRotate(cameraWorldTransform_.rotate);
	CameraManager::GetInstance()->FindCamera("titleCamera")->SetTranslate(cameraWorldTransform_.translate);
	//3Dオブジェクト
	object3d_[1]->SetParent(&object3d_[0]->GetWorldMatrix());
	for (int i = 0;i < 2;i++) {
		object3d_[i]->SetRotate(worldTransform3D_[i].rotate);
		object3d_[i]->SetTranslate(worldTransform3D_[i].translate);
		object3d_[i]->SetCamera(CameraManager::GetInstance()->FindCamera("titleCamera"));
		object3d_[i]->Update();
	}
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
	ImGui::DragFloat3("rotate[0]", &worldTransform3D_[0].rotate.x, 0.1f);
	ImGui::DragFloat3("rotate[1]", &worldTransform3D_[1].rotate.x, 0.1f);
	ImGui::DragFloat3("translate[0]", &worldTransform3D_[0].translate.x, 0.1f);
	ImGui::DragFloat3("translate[1]", &worldTransform3D_[1].translate.x, 0.1f);
	ImGui::End();

	ImGui::Begin("camera");
	ImGui::DragFloat3("rotate", &cameraWorldTransform_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &cameraWorldTransform_.translate.x, 0.1f);
	ImGui::End();
#endif // USE_IMGUI
	//ImGuiの受付終了
	ImGuiManager::GetInstance()->End();
}

//描画
void TitleScene::Draw() {
	//スプライト
	for (int i = 0;i < 2;i++) {
		object2d_[i]->Draw();
	}
	//3Dオブジェクト
	for (int i = 0;i < 2;i++) {
		object3d_[i]->Draw();
	}
}

//終了
void TitleScene::Finalize() {
}
