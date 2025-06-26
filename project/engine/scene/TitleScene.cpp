#include "TitleScene.h"
#include "engine/input/Input.h"
#include "SceneManager.h"
#include "SceneFactory.h"
#include "engine/3d/ModelManager.h"
#include "engine/3d/Object3dCommon.h"
#include "engine/math/func/Math.h"

//初期化
void TitleScene::Initialize(DirectXBase* directXBase) {
	object2d_ = std::make_unique<Object2d>();
	object2d_->Initialize("monsterBall.png");
	//object2d_->ChangeTexture("circle.png");
	worldTransform2d_ = {
		{100.0f,100.0f},
		Math::kPi,
		{}
	};
	color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("monsterBall.png");

	object3d_ = std::make_unique<Object3d>();
	worldTransform3d_.scale = { 1.0f,1.0f,1.0f };
	worldTransform3d_.rotate = {};
	worldTransform3d_.translate = { 0.0f,0.0f,0.0f };
	object3d_->Initialize();
	object3d_->SetModel("axis");
	object3d_->SetTexture("uvChecker");

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.intensity = 1.0f;

	particleSystem_ = std::make_unique<ParticleSystem>();
	particleSystem_->Initialize(directXBase, "uvChecker.png");
}

//更新
void TitleScene::Update() {
	//ImGuiの受付開始
	ImGuiManager::GetInstance()->Begin();
	object2d_->Update();
	object2d_->SetTransform(worldTransform2d_);

	object3d_->SetScale(worldTransform3d_.scale);
	object3d_->SetRotate(worldTransform3d_.rotate);
	object3d_->SetTranslate(worldTransform3d_.translate);
	object3d_->SetColor(object3dColor_);
	Object3dCommon::GetInstance()->SetDirectionalLightData(directionalLight_);
	object3d_->Update();

	sprite_->Update();

//	particleSystem_->Update();
#ifdef USE_IMGUI
	ImGui::Begin("2d");
	ImGui::DragFloat2("scale", &worldTransform2d_.scale.x, 0.1f);
	ImGui::DragFloat("rotate", &worldTransform2d_.rotate, 0.1f);
	ImGui::DragFloat2("translate", &worldTransform2d_.translate.x, 0.1f);
	ImGui::ColorEdit4("color", &color.x);
	ImGui::End();

	ImGui::Begin("3dModel");
	ImGui::DragFloat3("scale", &worldTransform3d_.scale.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform3d_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &worldTransform3d_.translate.x, 0.1f);
	ImGui::DragFloat4("color", &object3dColor_.x, 0.1f);
	ImGui::ColorEdit4("color", &object3dColor_.x);
	ImGui::End();

	ImGui::Begin("light");
	ImGui::ColorEdit4("color", &directionalLight_.color.x);
	ImGui::DragFloat3("direction", &directionalLight_.direction.x, 0.1f);
	ImGui::DragFloat("intensity", &directionalLight_.intensity,0.1f,0.0f,10.0f);
	ImGui::End();

	ImGui::Begin("blend");
	const char* blendModes3d[] = { "None", "Normal", "Add", "Subtract", "Multiply", "Screen" };
	if (ImGui::Combo("3dMode", &blendMode_, blendModes3d, IM_ARRAYSIZE(blendModes3d))) {
		object3d_->SetBlendMode((BlendMode)blendMode_);
	}
	/*const char* blendModes2d[] = { "None", "Normal", "Add", "Subtract", "Multiply", "Screen" };
	if (ImGui::Combo("2dMode", &blendMode_, blendModes2d, IM_ARRAYSIZE(blendModes2d))) {
		object2d_->SetBlendMode((BlendMode)blendMode_);
	}*/
	ImGui::End();
#endif // USE_IMGUI
	//ImGuiの受付終了
	ImGuiManager::GetInstance()->End();
}

//描画
void TitleScene::Draw() {
	object3d_->Draw();
	object2d_->Draw();
	sprite_->Draw();
	//particleSystem_->Draw();
}

//終了
void TitleScene::Finalize() {
}
