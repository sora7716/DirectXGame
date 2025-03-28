#include "Object3d.h"
#include "engine/objectCommon/Object3dCommon.h"
#include "engine/3d/Camera.h"
#include "engine/base/DirectXBase.h"
#include "engine/math/func/Math.h"
#include "ModelManager.h"
#include <fstream>
#include <sstream>
#include <cassert>
//初期化
void Object3d::Initialize(Object3dCommon* object3dCommon) {
	//Nullチェック
	assert(object3dCommon);
	//引数を受け取ってメンバ変数に記録
	object3dCommon_ = object3dCommon;
	//DirectXの基盤部分を受け取る
	directXBase_ = object3dCommon_->GetDirectXBase();
	//座標変換行列の生成
	CreateTransformationMatrixResource();
	//光源の生成
	CreateDirectionLight();
	//Transform変数を作る
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	uvTransform_ = { {1.0f,1.0f},0.0f,{0.0f,0.0f} };
	//カメラにデフォルトカメラを代入
	camera_ = object3dCommon_->GetDefaultCamera();
}

//更新
void Object3d::Update() {
	//ワールドマトリックスの生成
	Matrix4x4 worldMatrix = Rendering::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	//wvpの書き込み
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		wvpData_->WVP = worldMatrix * viewProjectionMatrix;
	}
	else {
		wvpData_->WVP = worldMatrix;
	}
	//worldTransformの書き込み
	wvpData_->World = worldMatrix;
	if (model_) {
		model_->UVTransform(uvTransform_);
	}
}

//描画
void Object3d::Draw() {
	//座標変換行列CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//平光源CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	//3Dモデルが割り当てられていれば描画
	if (model_) {
		model_->Draw();
	}
}

//モデルのセッター
void Object3d::SetModel(const std::string& filePath) {
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}

//カメラのセッター
void Object3d::SetCamera(Camera* camera) {
	camera_ = camera;
}

// スケールのセッター
void Object3d::SetScale(const Vector3& scale) {
	transform_.scale = scale;
}

// 回転のセッター
void Object3d::SetRotate(const Vector3& rotate) {
	transform_.rotate = rotate;
}

// 平行移動のセッター
void Object3d::SetTranslate(const Vector3& translate) {
	transform_.translate = translate;
}

// uvスケールのセッター
void Object3d::SetUVScale(const Vector2& uvScale) {
	uvTransform_.scale = uvScale;
}

// uv回転のセッター
void Object3d::SetUVRotate(const float uvRotate) {
	uvTransform_.rotate = uvRotate;
}

// uv平行移動のセッター
void Object3d::SetUVTranslate(const Vector2& uvTranslate) {
	uvTransform_.translate = uvTranslate;
}

//色のセッター
void Object3d::SetColor(const Vector4& color) {
	if (model_) {
		model_->SetColor(color);
	}
}

// スケールのゲッター
const Vector3& Object3d::GetScale() const {
	// TODO: return ステートメントをここに挿入します
	return transform_.scale;
}

// 回転のゲッター
const Vector3& Object3d::GetRotate() const {
	// TODO: return ステートメントをここに挿入します
	return transform_.rotate;
}

// 平行移動のゲッター
const Vector3& Object3d::GetTranslate() const {
	// TODO: return ステートメントをここに挿入します
	return transform_.translate;
}

// uvスケールのゲッター
const Vector2& Object3d::GetUVScale() const {
	// TODO: return ステートメントをここに挿入します
	return uvTransform_.scale;
}

// uv回転のゲッター
const float Object3d::GetUVRotate() const {
	// TODO: return ステートメントをここに挿入します
	return uvTransform_.rotate;
}

// uv平行移動のゲッター
const Vector2& Object3d::GetUVTranslate() const {
	// TODO: return ステートメントをここに挿入します
	return uvTransform_.translate;
}

//色のゲッター
const Vector4& Object3d::GetColor() const {
	// TODO: return ステートメントをここに挿入します
	static const Vector4 defaultColor(0, 0, 0, 0);
	if (model_) {
		return model_->GetColor();
	}
	return defaultColor;
}

// 座標変換行列リソースの生成
void Object3d::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る
	wvpResource_ = directXBase_->CreateBufferResource(sizeof(TransformationMatrix));
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	//単位行列を書き込んでおく
	wvpData_->WVP = Math::MakeIdentity4x4();
	wvpData_->World = Math::MakeIdentity4x4();
}

//光源の生成
void Object3d::CreateDirectionLight() {
	//光源のリソースを作成
	directionalLightResource_ = directXBase_->CreateBufferResource(sizeof(DirectionalLight));
	//光源データの書きこみ
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;
}
