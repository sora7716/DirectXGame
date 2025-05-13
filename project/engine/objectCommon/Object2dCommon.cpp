#include "Object2dCommon.h"
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