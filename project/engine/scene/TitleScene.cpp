#include "TitleScene.h"
#include "engine/input/Input.h"
#include "SceneManager.h"
#include "SceneFactory.h"
#include "engine/3d/ModelManager.h"
#include "engine/objectCommon/Object3dCommon.h"

//初期化
void TitleScene::Initialize(DirectXBase* directXBase) {
	object2d_ = std::make_unique<Object2d>();
	object2d_->Initialize();
	object2d_->SetSprite("block");
	worldTransform_.scale = { 100.0f,100.0f };
	worldTransform_.rotate = 0.0f;
	worldTransform_.translate = { 100.0f,100.0f };
	color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	object3d_ = std::make_unique<Object3d>();
	worldTransform3d_.scale = { 1.0f,1.0f,1.0f };
	worldTransform3d_.rotate = {};
	worldTransform3d_.translate = { 0.0f,0.0f,0.0f };
	object3d_->Initialize();
	object3d_->SetModel("cube");
	object3d_->SetTexture("uvChecker");

	for (int i = 0; i < 2; i++) {
		plane_[i] = std::make_unique<PlaneObject>();
		plane_[i]->Initialize();
		plane_[i]->SetTexture("block");
	}
	planeTransform_[0].scale = { 100.0f,100.0f };
	planeTransform_[0].rotate = 0.0f;
	planeTransform_[0].translate = { 100.0f,100.0f };
	planeTransform_[1].scale = { 1.0f,1.0f };
	planeTransform_[1].rotate = 0.0f;
	planeTransform_[1].translate = { 2.0f,0.0f };

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.intensity = 1.0f;

	particleEmit_ = std::make_unique<ParticleEmit>();
	particleEmit_->Initialize(directXBase);
}

//更新
void TitleScene::Update() {
	//ImGuiの受付開始
	ImGuiManager::GetInstance()->Begin();
	object2d_->SetTransform(worldTransform_);
	object2d_->SetColor(color);
	object2d_->Update();

	object3d_->SetScale(worldTransform3d_.scale);
	object3d_->SetRotate(worldTransform3d_.rotate);
	object3d_->SetTranslate(worldTransform3d_.translate);
	object3d_->SetColor(object3dColor_);
	Object3dCommon::GetInstance()->SetDirectionalLightData(directionalLight_);
	object3d_->Update();

	for (int i = 0; i < 2; i++) {
		plane_[i]->SetTransform(planeTransform_[i]);

	}
	plane_[0]->Update();
	plane_[1]->Update();
	if (Input::GetInstance()->TriggerKey(DIK_W)) {
		plane_[1]->SetParent(plane_[0]->GetWorldTransform());
	}
	else if (Input::GetInstance()->TriggerKey(DIK_S)) {
		plane_[1]->SetParent(nullptr);
	}

	particleEmit_->Update();

#ifdef USE_IMGUI
	ImGui::Begin("sprite");
	ImGui::DragFloat2("scale", &worldTransform_.scale.x, 0.1f);
	ImGui::DragFloat("rotate", &worldTransform_.rotate, 0.1f);
	ImGui::DragFloat2("translate", &worldTransform_.translate.x, 0.1f);
	ImGui::ColorEdit4("color", &color.x);
	ImGui::End();

	ImGui::Begin("quad");
	ImGui::DragFloat2("scale[0]", &planeTransform_[0].scale.x, 0.1f);
	ImGui::DragFloat("rotate[0]", &planeTransform_[0].rotate, 0.1f);
	ImGui::DragFloat2("translate[0]", &planeTransform_[0].translate.x, 0.1f);
	ImGui::DragFloat2("scale[1]", &planeTransform_[1].scale.x, 0.1f);
	ImGui::DragFloat("rotate[1]", &planeTransform_[1].rotate, 0.1f);
	ImGui::DragFloat2("translate[1]", &planeTransform_[1].translate.x, 0.1f);
	ImGui::End();

	ImGui::Begin("3dModel");
	ImGui::DragFloat3("scale", &worldTransform3d_.scale.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform3d_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &worldTransform3d_.translate.x, 0.1f);
	ImGui::DragFloat4("color", &object3dColor_.x, 0.1f);
	ImGui::ColorEdit4("color", &object3dColor_.x);
	ImGui::End();

	ImGui::Begin("suji");
	ImGui::End();

	ImGui::Begin("light");
	ImGui::ColorEdit4("color", &directionalLight_.color.x);
	ImGui::DragFloat3("direction", &directionalLight_.direction.x, 0.1f);
	ImGui::DragFloat("intensity", &directionalLight_.intensity,0.1f,0.0f,10.0f);
	ImGui::End();

	ImGui::Begin("blend");
	/*const char* blendModes3d[] = { "None", "Normal", "Add", "Subtract", "Multiply", "Screen" };
	if (ImGui::Combo("3dMode", &blendMode_, blendModes3d, IM_ARRAYSIZE(blendModes3d))) {
		object3d_->SetBlendMode((BlendMode)blendMode_);
	}*/
	const char* blendModes2d[] = { "None", "Normal", "Add", "Subtract", "Multiply", "Screen" };
	if (ImGui::Combo("2dMode", &blendMode_, blendModes2d, IM_ARRAYSIZE(blendModes2d))) {
		object2d_->SetBlendMode((BlendMode)blendMode_);
	}
	ImGui::End();
#endif // USE_IMGUI
	//ImGuiの受付終了
	ImGuiManager::GetInstance()->End();
}

//描画
void TitleScene::Draw() {
	object3d_->Draw();
	object2d_->Draw();
	for (int i = 0; i < 2; i++) {
		//plane_[i]->Draw();
	}
	particleEmit_->Draw();
}

//終了
void TitleScene::Finalize() {
	particleEmit_->Finalize();
}
