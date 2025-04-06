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
	worldTransform3d_.translate = {0.0f,0.0f,0.0f};
	object3d_->Initialize();
	object3d_->SetModel("cube");
	
	plane_ = std::make_unique<PlaneObject>();
	plane_->Initialize();
	plane_->SetTexture("block");
	planeTransform_.scale = { 100.0f,100.0f };
	planeTransform_.rotate = 0.0f;
	planeTransform_.translate = { 100.0f,100.0f };
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
	object3d_->Update();

	plane_->SetTransform(planeTransform_);
	plane_->Update();
#ifdef USE_IMGUI
	ImGui::Begin("sprite");
	ImGui::DragFloat2("scale", &worldTransform_.scale.x, 0.1f);
	ImGui::DragFloat("rotate", &worldTransform_.rotate, 0.1f);
	ImGui::DragFloat2("translate", &worldTransform_.translate.x, 0.1f);
	ImGui::ColorEdit4("color", &color.x);
	ImGui::End();

	ImGui::Begin("quad");
	ImGui::DragFloat2("scale", &planeTransform_.scale.x, 0.1f);
	ImGui::DragFloat("rotate", &planeTransform_.rotate, 0.1f);
	ImGui::DragFloat2("translate", &planeTransform_.translate.x, 0.1f);
	ImGui::End();

	ImGui::Begin("3dModel");
	ImGui::DragFloat3("scale", &worldTransform3d_.scale.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform3d_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &worldTransform3d_.translate.x, 0.1f);
	ImGui::End();
#endif // USE_IMGUI
	//ImGuiの受付終了
	ImGuiManager::GetInstance()->End();
}

//描画
void TitleScene::Draw() {
	object2d_->Draw();
	object3d_->Draw();
	plane_->Draw();
}

//終了
void TitleScene::Finalize() {
}
