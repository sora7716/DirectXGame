#include "Object3dCommon.h"
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