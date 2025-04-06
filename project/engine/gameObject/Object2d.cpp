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
	object2dCommon_ = Object2dCommon::GetInstance();
	directXBase_ = object2dCommon_->GetDirectXBase();
	//光源の生成
	CreateDirectionLight();
	//uvTransform変数を作る
	uvTransform_ = { {1.0f,1.0f},0.0f,{0.0f,0.0f} };
	//ワールドトランスフォームの生成と初期化
	worldTransform2d_ = std::make_unique<WorldTransform2d>();
	worldTransform2d_->Initialize(directXBase_);
	//カメラにデフォルトカメラを代入
	worldTransform2d_->camera_ = object2dCommon_->GetDefaultCamera();
}

//更新
void Object2d::Update() {
	//ワールドトランスフォームの更新
	worldTransform2d_->Update();
	//UV座標の更新
	if (sprite_) {
		sprite_->UpdateUVTransform(uvTransform_);
	}
}

//描画
void Object2d::Draw() {
	//ワールドトランスフォームの描画
	worldTransform2d_->Draw();
	//平光源CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	//3Dモデルが割り当てられていれば描画
	if (sprite_) {
		sprite_->Draw();
	}
}

//スプライトのセッター
void Object2d::SetSprite(const std::string&name) {
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
	return worldTransform2d_->transform2d_.scale;
}

//回転のゲッター
float Object2d::GetRotate() const {
	return worldTransform2d_->transform2d_.rotate;
}

//位置のゲッター
const Vector2& Object2d::GetTranslate() const {
	// TODO: return ステートメントをここに挿入します
	return worldTransform2d_->transform2d_.translate;
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

//カメラのゲッター
const Camera* Object2d::GetCamera() const {
	return worldTransform2d_->camera_;
}

//ワールドトランスフォームのゲッター
WorldTransform2d* Object2d::GetWorldTransform(){
	return worldTransform2d_.get();
}


//サイズのセッター
void Object2d::SetScale(const Vector2& scale) {
	worldTransform2d_->transform2d_.scale = scale;
}

//回転のセッター
void Object2d::SetRotate(float rotate) {
	worldTransform2d_->transform2d_.rotate = rotate;
}

//位置のセッター
void Object2d::SetTranslate(const Vector2& translate) {
	worldTransform2d_->transform2d_.translate = translate;
}

//トランスフォームのセッター
void Object2d::SetTransform(const Transform2d& transform2d){
	worldTransform2d_->transform2d_ = transform2d;
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
	worldTransform2d_->camera_ = camera;
}

//親のセッター
void Object2d::SetParent(const WorldTransform2d* parent) {
	worldTransform2d_->parent_ = parent;
}

//光源の生成
void Object2d::CreateDirectionLight() {
	//光源
	directionalLightResource_ = directXBase_->CreateBufferResource(sizeof(DirectionalLight));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;
}
