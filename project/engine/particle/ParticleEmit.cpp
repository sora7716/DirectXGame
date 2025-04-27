#include "ParticleEmit.h"
#include "engine/base/DirectXBase.h"
#include "engine/2d/SpriteManager.h"
#include "engine/gameObject/Object2d.h"

//初期化
void ParticleEmit::Initialize(DirectXBase* directXBase) {
	//DirectXの基盤部分を記録する
	directXBase_ = directXBase;
	//スプライトの生成
	object2d_ = new Object2d();
	object2d_->Initialize();
	object2d_->SetSprite("uvChecker");
	//ワールドトランスフォームの設定
	worldTransform_ = {
		.scale = {100.0f,100.0f},
		.rotate = 0.0f,
		.translate = {0.0f,0.0f}
	};
}

//更新
void ParticleEmit::Update() {
	object2d_->SetTransform(worldTransform_);
	//オブジェクト2d
	object2d_->Update();
}

//描画
void ParticleEmit::Draw() {
	//オブジェクト2d
	object2d_->Draw();
}

//終了
void ParticleEmit::Finalize() {
	delete object2d_;
}


