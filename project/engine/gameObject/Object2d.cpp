#include "Object2d.h"
#include "engine/objectCommon/Object2dCommon.h"
#include "engine/base/DirectXBase.h"
#include "engine/base/WinApi.h"
#include "engine/math/func/Math.h"
#include "engine/gameObject/Camera.h"
#include "SpriteManager.h"
#include "Sprite.h"
//初期化
void Object2d::Initialize() {
	//DirectXの基盤部分を記録する
	directXBase_ = Object2dCommon::GetInstance()->GetDirectXBase();
	//光源の生成
	Object2dCommon::GetInstance()->CreateDirectionLight();
	//uvTransform変数を作る
	uvTransform_ = { {1.0f,1.0f},0.0f,{0.0f,0.0f} };
	//ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize(directXBase_, TransformMode::k2d);
	//カメラにデフォルトカメラを代入
	worldTransform_->SetCamera(Object2dCommon::GetInstance()->GetDefaultCamera());
	//スクリーンに表示する範囲を設定
	WorldTransform::ScreenArea screenArea = {
		.left = 0,
		.top = 0,
		.right = (float)WinApi::kClientWidth,
		.bottom = (float)WinApi::kClientHeight,
	};
	worldTransform_->SetScreenArea(screenArea);
}

//更新
void Object2d::Update() {
	//ワールドトランスフォームの更新
	worldTransform_->Update();
	//UV座標の更新
	if (sprite_) {
		sprite_->UpdateUVTransform(uvTransform_);
	}
}

//描画
void Object2d::Draw() {
	//PSOの設定
	auto pso = Object2dCommon::GetInstance()->GetGraphicsPipelineStates()[static_cast<int32_t>(blendMode_)].Get();
	//グラフィックスパイプラインをセットするコマンド
	directXBase_->GetCommandList()->SetPipelineState(pso);
	//ワールドトランスフォームの描画
	worldTransform_->Draw();
	//平光源CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(3, Object2dCommon::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
	//3Dモデルが割り当てられていれば描画
	if (sprite_) {
		sprite_->Draw();
	}
}

//スプライトのセッター
void Object2d::SetSprite(const std::string& name) {
	sprite_ = SpriteManager::GetInstance()->FindSprite(name);
}

//テクスチャの変更
void Object2d::ChangeTexture(std::string spriteName) {
	if (sprite_) {
		sprite_->ChangeTexture(spriteName);
	}
}

//サイズのゲッター
const Vector2& Object2d::GetScale() const {
	// TODO: return ステートメントをここに挿入します
	Vector2* result = {};
	*result = worldTransform_->GetScale();
	return *result;
}

//回転のゲッター
float Object2d::GetRotate() const {
	return worldTransform_->GetRotate().z;
}

//位置のゲッター
const Vector2& Object2d::GetTranslate() const {
	// TODO: return ステートメントをここに挿入します
	Vector2* result = {};
	*result = worldTransform_->GetTranslate();
	return *result;
}

//UVのサイズのゲッター
const Vector2& Object2d::GetUVScale() const {
	// TODO: return ステートメントをここに挿入します
	return uvTransform_.scale;
}

//UVの回転のゲッター
float Object2d::GetUVRotate() const {
	return uvTransform_.rotate;
}

//UVの位置のゲッター
const Vector2& Object2d::GetUVTranslate() const {
	// TODO: return ステートメントをここに挿入します
	return uvTransform_.translate;
}

//色のゲッター
const Vector4& Object2d::GetColor() const {
	// TODO: return ステートメントをここに挿入します
	static Vector4 defaultColor = {};
	if (sprite_) {
		return sprite_->GetColor();
	}
	return defaultColor;
}

//ワールドトランスフォームのゲッター
WorldTransform* Object2d::GetWorldTransform() {
	return worldTransform_.get();
}


//サイズのセッター
void Object2d::SetScale(const Vector2& scale) {
	worldTransform_->SetScale({ scale.x,scale.y,1.0f });
}

//回転のセッター
void Object2d::SetRotate(float rotate) {
	worldTransform_->SetRotate({ 0.0f,0.0f,rotate });
}

//位置のセッター
void Object2d::SetTranslate(const Vector2& translate) {
	worldTransform_->SetTranslate({ translate.x,translate.y,0.0f });
}

//トランスフォームのセッター
void Object2d::SetTransform(const Transform2d& transform2d) {
	worldTransform_->SetTransform2d(transform2d);
}

//UVのサイズのセッター
void Object2d::SetUVScale(const Vector2& scale) {
	uvTransform_.scale = scale;
}

//UVの回転のセッター
void Object2d::SetUVRotate(float rotate) {
	uvTransform_.rotate = rotate;
}

//UVの位置のゲッター
void Object2d::SetUVTranslate(const Vector2& translate) {
	uvTransform_.translate = translate;
}

//色のセッター
void Object2d::SetColor(const Vector4& color) {
	if (sprite_) {
		sprite_->SetColor(color);
	}
}

//カメラのセッター
void Object2d::SetCamera(Camera* camera) {
	worldTransform_->SetCamera(camera);
}

//親のセッター
void Object2d::SetParent(const WorldTransform* parent) {
	worldTransform_->SetParent(parent);
}

//ブレンドモードのセッター
void Object2d::SetBlendMode(const BlendMode& blendMode){
	blendMode_ = blendMode;
}
