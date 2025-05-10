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

//初期化
void SpriteCommon::Initialize(DirectXBase* directXBase) {
	//Nullチェック
	assert(directXBase);
	//引数を受け取ってメンバ変数に記録する
	directXBase_ = directXBase;
}

//テクスチャの読み込み
void SpriteCommon::LoadTexture(const std::string& filename) {
	TextureManager::GetInstance()->LoadTexture(filename);
}

//DirectXの基盤部分のゲッター
DirectXBase* SpriteCommon::GetDirectXBase(){
	return directXBase_;
}

//終了
void SpriteCommon::Finalize(){
	delete instance;
	instance = nullptr;
	isFinalize = true;
}
