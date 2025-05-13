#include "PlaneObject.h"
#include "engine/objectCommon/Object2dCommon.h"
#include "engine/gameObject/Camera.h"
#include "engine/base/DirectXBase.h"
#include "engine/math/func/Math.h"
#include "engine/3d/ModelManager.h"
#include "engine/base/WinApi.h"
#include <cassert>
//初期化
void PlaneObject::Initialize() {
	//DirectXの基盤部分を受け取る
	directXBase_ = Object2dCommon::GetInstance()->GetDirectXBase();
	//光源の生成
	Object2dCommon::GetInstance()->CreateDirectionLight();
	//光源の値の設定
	directionalLightData_ = { { 1.0f,1.0f,1.0f,1.0f } ,{ 0.0f,0.0f,1.0f } ,1.0f };
	Object2dCommon::GetInstance()->SetDirectionalLightData(directionalLightData_);
	//uvTransform変数を作る
	uvTransform_ = { {1.0f,1.0f},0.0f,{0.0f,0.0f} };
	//ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize(directXBase_, TransformMode::k2d);
	//カメラにデフォルトカメラを代入
	worldTransform_->SetCamera(Object2dCommon::GetInstance()->GetDefaultCamera());
	//モデルの生成
	model_ = ModelManager::GetInstance()->FindModel("plane");
	//スクリーンに表示する範囲を設定
	WorldTransform::ScreenArea screenArea = {
		.left = -(float)WinApi::kClientWidth / 2.0f,
		.top = (float)WinApi::kClientHeight / 2.0f,
		.right = (float)WinApi::kClientWidth / 2.0f,
		.bottom = -(float)WinApi::kClientHeight / 2.0f,
	};
	worldTransform_->SetScreenArea(screenArea);
}

//更新
void PlaneObject::Update() {
	//ワールドトランスフォームの更新
	worldTransform_->Update();
	if (model_) {
		model_->UVTransform(uvTransform_);
	}
}

//描画
void PlaneObject::Draw() {
	//ワールドトランスフォームの描画
	worldTransform_->Draw();
	//平光源CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(3, Object2dCommon::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
	//3Dモデルが割り当てられていれば描画
	if (model_) {
		model_->Draw();
	}
}

//モデルのセッター
void PlaneObject::SetModel(const std::string& name) {
	model_ = ModelManager::GetInstance()->FindModel(name);
}

//カメラのセッター
void PlaneObject::SetCamera(Camera* camera) {
	worldTransform_->SetCamera(camera);
}

// スケールのセッター
void PlaneObject::SetScale(const Vector2& scale) {
	worldTransform_->SetScale({ scale.x,scale.y,1.0f });
}

// 回転のセッター
void PlaneObject::SetRotate(float rotate) {
	worldTransform_->SetRotate({ 0.0f,0.0f,rotate });
}

// 平行移動のセッター
void PlaneObject::SetTranslate(const Vector2& translate) {
	worldTransform_->SetTranslate({ translate.x,translate.y,0.0f });
}

//トランスフォームのセッター
void PlaneObject::SetTransform(const Transform2d& transform2d) {
	worldTransform_->SetTransform2d(transform2d);
}

// uvスケールのセッター
void PlaneObject::SetUVScale(const Vector2& uvScale) {
	uvTransform_.scale = uvScale;
}

// uv回転のセッター
void PlaneObject::SetUVRotate(const float uvRotate) {
	uvTransform_.rotate = uvRotate;
}

// uv平行移動のセッター
void PlaneObject::SetUVTranslate(const Vector2& uvTranslate) {
	uvTransform_.translate = uvTranslate;
}

//色のセッター
void PlaneObject::SetColor(const Vector4& color) {
	if (model_) {
		model_->SetColor(color);
	}
}

//親のセッター
void PlaneObject::SetParent(const WorldTransform* parent) {
	worldTransform_->SetParent(parent);
}

//テクスチャの変更
void PlaneObject::SetTexture(const std::string& filePath) {
	if (model_) {
		model_->SetTexture("engine/resources/textures/" + filePath + ".png");
	}
}

// スケールのゲッター
const Vector2& PlaneObject::GetScale() const {
	// TODO: return ステートメントをここに挿入します
	Vector2* result = {};
	*result = worldTransform_->GetScale();
	return *result;
}

// 回転のゲッター
const float PlaneObject::GetRotate() const {
	return worldTransform_->GetRotate().z;
}

// 平行移動のゲッター
const Vector2& PlaneObject::GetTranslate() const {
	// TODO: return ステートメントをここに挿入します
	Vector2* result = {};
	*result = worldTransform_->GetTranslate();
	return *result;
}

// uvスケールのゲッター
const Vector2& PlaneObject::GetUVScale() const {
	// TODO: return ステートメントをここに挿入します
	return uvTransform_.scale;
}

// uv回転のゲッター
const float PlaneObject::GetUVRotate() const {
	// TODO: return ステートメントをここに挿入します
	return uvTransform_.rotate;
}

// uv平行移動のゲッター
const Vector2& PlaneObject::GetUVTranslate() const {
	// TODO: return ステートメントをここに挿入します
	return uvTransform_.translate;
}

//色のゲッター
const Vector4& PlaneObject::GetColor() const {
	// TODO: return ステートメントをここに挿入します
	static const Vector4 defaultColor(0, 0, 0, 0);
	if (model_) {
		return model_->GetColor();
	}
	return defaultColor;
}

//ワールドトランスフォームのゲッター
const WorldTransform* PlaneObject::GetWorldTransform() const {
	// TODO: return ステートメントをここに挿入します
	return worldTransform_.get();
}