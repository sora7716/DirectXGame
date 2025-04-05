#include "TitleScene.h"
#include "engine/input/Input.h"
#include "SceneManager.h"
#include "SceneFactory.h"
#include "engine/3d/ModelManager.h"

//初期化
void TitleScene::Initialize(DirectXBase* directXBase) {
	object2d_ = std::make_unique<Object2d>();
	object2d_->Initialize();
	object2d_->SetSprite(SpriteManager::GetInstance()->FindSprite("block"));
	worldTransform_.scale = { 100.0f,100.0f };
	worldTransform_.rotate = 0.0f;
	worldTransform_.translate = { 100.0f,100.0f };
	color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

//更新
void TitleScene::Update() {
	//ImGuiの受付開始
	ImGuiManager::GetInstance()->Begin();
	object2d_->SetTransform(worldTransform_);
	object2d_->SetColor(color);
	object2d_->Update();
#ifdef USE_IMGUI
	ImGui::Begin("sprite");
	ImGui::DragFloat2("scale", &worldTransform_.scale.x, 0.1f);
	ImGui::DragFloat("rotate", &worldTransform_.rotate, 0.1f);
	ImGui::DragFloat2("translate", &worldTransform_.translate.x, 0.1f);
	ImGui::ColorEdit4("color", &color.x);
	ImGui::End();
#endif // USE_IMGUI
	//ImGuiの受付終了
	ImGuiManager::GetInstance()->End();
}

//描画
void TitleScene::Draw() {
	object2d_->Draw();
}

//終了
void TitleScene::Finalize() {
}
