#include "Object2dCommon.h"
#include "engine/gameObject/Camera.h"
#include <cassert>

//インスタンスのゲッター
Object2dCommon* Object2dCommon::GetInstance() {
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new Object2dCommon();
	}
	return instance;
}

//終了
void Object2dCommon::Finalize() {
	delete instance;
	instance = nullptr;
	isFinalize = true;
}

// デフォルトカメラのセッター
void Object2dCommon::SetDefaultCamera(Camera* camera) {
	defaultCamera_ = camera;
}

// デフォルトカメラのゲッター
Camera* Object2dCommon::GetDefaultCamera() const {
	return defaultCamera_;
}
