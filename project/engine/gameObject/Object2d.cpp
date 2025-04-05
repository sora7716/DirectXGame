#include "Object2d.h"
#include "engine/objectCommon/Object2dCommon.h"
#include "engine/base/DirectXBase.h"
#include "engine/base/WinApi.h"
#include "engine/math/func/Math.h"
#include "engine/gameObject/Camera.h"
#include "Sprite.h"
//初期化
void Object2d::Initialize() {
	object2dCommon_ = Object2dCommon::GetInstance();
	directXBase_ = object2dCommon_->GetDirectXBase();
	//座標変換行列データの生成
	CreateTransformationMatrixResorce();
	//光源の生成
	CreateDirectionLight();
	//Transform変数を作る
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	uvTransform_ = { {1.0f,1.0f},0.0f,{0.0f,0.0f} };
	//カメラにデフォルトカメラを代入
	camera_ = object2dCommon_->GetDefaultCamera();
}

//更新
void Object2d::Update() {
	//メンバ変数の値を見た目に反映
	transform_.scale = { transform2d_.scale.x,transform2d_.scale.y,1.0f };
	transform_.rotate = { 0.0f,0.0f,transform2d_.rotate };
	transform_.translate = { transform2d_.translate.x,transform2d_.translate.y,0.0f };
	//座標の更新
	UpdateTransform();
	//UV座標の更新
	if (sprite_) {
		sprite_->UpdateUVTransform(uvTransform_);
	}
}

//描画
void Object2d::Draw() {
	//座標変換行列CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResoruce_->GetGPUVirtualAddress());//wvp
	//平光源CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	//3Dモデルが割り当てられていれば描画
	if (sprite_) {
		sprite_->Draw();
	}
}

//スプライトのセッター
void Object2d::SetSprite(Sprite* sprite) {
	sprite_ = sprite;
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
	return transform2d_.scale;
}

//回転のゲッター
float Object2d::GetRotate() const {
	return transform2d_.rotate;
}

//位置のゲッター
const Vector2& Object2d::GetTranslate() const {
	// TODO: return ステートメントをここに挿入します
	return transform2d_.translate;
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
	return camera_;
}

//ワールド行列のゲッター
const Matrix4x4& Object2d::GetWorldMatrix() const {
	// TODO: return ステートメントをここに挿入します
	return wvpData_->World;
}

//サイズのセッター
void Object2d::SetScale(const Vector2& scale) {
	transform2d_.scale = scale;
}

//回転のセッター
void Object2d::SetRotate(float rotate) {
	transform2d_.rotate = rotate;
}

//位置のセッター
void Object2d::SetTranslate(const Vector2& translate) {
	transform2d_.translate = translate;
}

//トランスフォームのセッター
void Object2d::SetTransform(const Transform2d& transform2d){
	transform2d_ = transform2d;
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
	camera_ = camera;
}

//親のセッター
void Object2d::SetParent(const Matrix4x4* parent) {
	parent_ = parent;
}


//座標変換行列リソースの生成
void Object2d::CreateTransformationMatrixResorce() {
	//座標変換行列リソースを作成する
	wvpResoruce_ = directXBase_->CreateBufferResource(sizeof(TransformationMatrix));
	//座標変換行列リソースにデータを書き込むためのアドレスを取得してtransformationMatrixDataに割り当てる
	//書き込むためのアドレス
	wvpResoruce_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	//単位行列を書き込んでおく
	wvpData_->WVP = Math::MakeIdentity4x4();
	wvpData_->World = Math::MakeIdentity4x4();
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

//座標変換の更新
void Object2d::UpdateTransform() {
	//TransformからWorldMatrixを作る
	Matrix4x4 worldMatrix = Rendering::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	if (parent_) {
		worldMatrix = worldMatrix * parent_;
	}
	//ProjectionMatrixを作って平行投影行列を書き込む
	Matrix4x4 projectionMatrix = Rendering::MakeOrthographicMatrix(0.0f, 0.0f, (float)WinApi::kClientWidth, (float)WinApi::kClientHeight, 0.0f, 100.0f);
	//wvpの書き込み
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewMatrix() * projectionMatrix;
		wvpData_->WVP = worldMatrix * viewProjectionMatrix;
	}
	else {
		wvpData_->WVP = worldMatrix * projectionMatrix;
	}
	//ワールド行列を送信
	wvpData_->World = worldMatrix;
}

