#include "Object3dCommon.h"
#include "engine/gameObject/Camera.h"
#include <cassert>

//インスタンスのゲッター
Object3dCommon* Object3dCommon::GetInstance(){
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new Object3dCommon();
	}
	return instance;
}

//終了
void Object3dCommon::Finalize(){
	delete instance;
	instance = nullptr;
	isFinalize = true;
}

// デフォルトカメラのセッター
void Object3dCommon::SetDefaultCamera(Camera* camera){
	defaultCamera_ = camera;
}

// デフォルトカメラのゲッター
Camera* Object3dCommon::GetDefaultCamera() const{
	return defaultCamera_;
}
