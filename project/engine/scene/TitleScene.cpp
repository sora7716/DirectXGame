#include "TitleScene.h"
#include "engine/input/Input.h"
#include "SceneManager.h"
#include "SceneFactory.h"
#include "engine/3d/ModelManager.h"

//初期化
void TitleScene::Initialize(DirectXBase* directXBase) {
	
}

//更新
void TitleScene::Update() {
	if (Input::GetInstance()->PressMouseButton(kLeft)) {
		isPressMouseButton_ ^= true;
	}
	mouseMove_ = Input::GetInstance()->GetMouseMoveAmount();
	wheelRotate_ = Input::GetInstance()->GetWheelRotate();
	if (Input::GetInstance()->TriggerMouseButton(kRight)) {
		isTriggerMouseButton_ ^= true;
	}
	if (Input::GetInstance()->ReleaseTriggerMouseButton(kMiddle)) {
		isReleseTriggerMouseButton_ ^= true;
	}
	worldMosePos_ = Input::GetInstance()->GetWorldMousePosition(CameraManager::GetInstance()->FindCamera("titleCamera"));
	screenMousePos_ = Input::GetInstance()->GetMousePosition();
	//ImGuiの受付開始
	ImGuiManager::GetInstance()->Begin();
#ifdef USE_IMGUI
	ImGui::Begin("mouse");
	ImGui::Text("move\nx:%d,y:%d", mouseMove_.x, mouseMove_.y);
	ImGui::Text("wheel:%d", wheelRotate_);
	ImGui::Checkbox("press", &isPressMouseButton_);
	ImGui::Checkbox("trigger", &isTriggerMouseButton_);
	ImGui::Checkbox("releseTrigger", &isReleseTriggerMouseButton_);
	ImGui::DragFloat3("mousePosW", &worldMosePos_.x, 0.1f);
	ImGui::DragInt2("mousePosSc", &screenMousePos_.x, 0.1f);
	ImGui::End();
#endif // USE_IMGUI
	//ImGuiの受付終了
	ImGuiManager::GetInstance()->End();
}

//描画
void TitleScene::Draw() {
	
}

//終了
void TitleScene::Finalize() {
}
