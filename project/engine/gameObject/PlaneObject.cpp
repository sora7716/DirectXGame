#include "PlaneObject.h"
#include "engine/objectCommon/Object3dCommon.h"
#include "engine/gameObject/Camera.h"
#include "engine/base/DirectXBase.h"
#include "engine/math/func/Math.h"
#include "engine/3d/ModelManager.h"
#include "engine/base/WinApi.h"
#include <cassert>
//初期化
void PlaneObject::Initialize() {
	//DirectXの基盤部分を受け取る
	directXBase_ = Object3dCommon::GetInstance()->GetDirectXBase();
	//座標変換行列の生成
	CreateTransformationMatrixResource();
	//光源の生成
	CreateDirectionLight();
	//Transform変数を作る
	transform2d_ = { {1.0f,1.0f},0.0f,{0.0f,0.0f} };
	uvTransform_ = { {1.0f,1.0f},0.0f,{0.0f,0.0f} };
	//カメラにデフォルトカメラを代入
	camera_ = Object3dCommon::GetInstance()->GetDefaultCamera();
	//モデルの生成
	model_ = ModelManager::GetInstance()->FindModel("plane");
	//親のワールド座標を保存
	saveWorldMatrix_ = Math::MakeIdentity4x4();
}

//更新
void PlaneObject::Update() {
	//トランスフォーム
	Transform transform;
	transform.scale = { transform2d_.scale.x,transform2d_.scale.y,1.0f };
	transform.rotate = { 0.0f,0.0f,transform2d_.rotate };
	transform.translate = { transform2d_.translate.x,transform2d_.translate.y,1.0f };
	//ワールドマトリックスの生成
	Matrix4x4 worldMatrix = Rendering::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	//親子付け
	if (parent_) {
		worldMatrix = worldMatrix * parent_;
		saveWorldMatrix_ = *parent_;
	}
	else {
		worldMatrix = worldMatrix * saveWorldMatrix_;
	}
	//wvpの書き込み
	if (camera_) {
		//画面を切り取るところ
		PlanePoint vertex = {
			-(float)WinApi::kClientWidth / 2.0f,
			(float)WinApi::kClientHeight / 2.0f,
			(float)WinApi::kClientWidth / 2.0f,
			-(float)WinApi::kClientHeight / 2.0f
		};
		const Matrix4x4& projectionMatrix = Rendering::MakeOrthographicMatrix(vertex.left, vertex.top, vertex.right, vertex.bottom, 0.0f, 100.0f);
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewMatrix() * projectionMatrix;
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
void PlaneObject::Draw() {
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
void PlaneObject::SetModel(const std::string& name) {
	model_ = ModelManager::GetInstance()->FindModel(name);
}

//カメラのセッター
void PlaneObject::SetCamera(Camera* camera) {
	camera_ = camera;
}

// スケールのセッター
void PlaneObject::SetScale(const Vector2& scale) {
	transform2d_.scale = scale;
}

// 回転のセッター
void PlaneObject::SetRotate(float rotate) {
	transform2d_.rotate = rotate;
}

// 平行移動のセッター
void PlaneObject::SetTranslate(const Vector2& translate) {
	transform2d_.translate = translate;
}

//トランスフォームのセッター
void PlaneObject::SetTransform(const Transform2d& transform2d) {
	transform2d_ = transform2d;
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
void PlaneObject::SetParent(const Matrix4x4* parent) {
	parent_ = parent;
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
	return transform2d_.scale;
}

// 回転のゲッター
const float PlaneObject::GetRotate() const {
	return transform2d_.rotate;
}

// 平行移動のゲッター
const Vector2& PlaneObject::GetTranslate() const {
	// TODO: return ステートメントをここに挿入します
	return transform2d_.translate;
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

//ワールド行列のゲッター
const Matrix4x4& PlaneObject::GetWorldMatrix() const {
	// TODO: return ステートメントをここに挿入します
	return wvpData_->World;
}

// 座標変換行列リソースの生成
void PlaneObject::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る
	wvpResource_ = directXBase_->CreateBufferResource(sizeof(TransformationMatrix));
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	//単位行列を書き込んでおく
	wvpData_->WVP = Math::MakeIdentity4x4();
	wvpData_->World = Math::MakeIdentity4x4();
}

//光源の生成
void PlaneObject::CreateDirectionLight() {
	//光源のリソースを作成
	directionalLightResource_ = directXBase_->CreateBufferResource(sizeof(DirectionalLight));
	//光源データの書きこみ
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,0.0f,1.0f };
	directionalLightData_->intensity = 1.0f;
}
