#include "SpriteCommon.h"
#include "engine/2d/TextureManager.h"
#include <cassert>

//インスタンスのゲッター
SpriteCommon* SpriteCommon::GetInstance(){
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new SpriteCommon();
	}
	return instance;
}

//テクスチャの読み込み
void SpriteCommon::LoadTexture(const std::string& filename) {
	TextureManager::GetInstance()->LoadTexture(filename);
}

//終了
void SpriteCommon::Finalize(){
	delete instance;
	instance = nullptr;
	isFinalize = true;
}
