#include "ModelCommon.h"
#include "engine/base/DirectXBase.h"
#include <cassert>

//初期化
void ModelCommon::Initialize(DirectXBase* directXBase){
	//Nullチェック
	assert(directXBase);
	//引数を受け取ってメンバ変数に記録する
	directXBase_ = directXBase;
}

// DirectXの基盤部分のゲッター
DirectXBase* ModelCommon::GetDirectXBase() const{
	return directXBase_;
}
