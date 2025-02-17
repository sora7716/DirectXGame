#include "Sprite.h"
#include "SpriteGeneral.h"
#include "engine/base/DirectXBase.h"
#include <cassert>

//初期化
void Sprite::Initialize(SpriteGeneral* spriteGeneral){
	assert(spriteGeneral);//Nullチェック
	spriteGeneral_ = spriteGeneral;//共通部分を受け取る
	directXBase_ = DirectXBase::GetInstance();//DirectXの基盤部分を受け取る
}
