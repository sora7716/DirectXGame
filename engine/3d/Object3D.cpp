#include "Object3d.h"
#include "engine/objectCommon/Object3dCommon.h"
#include "engine/2d/TextureManager.h"
#include "engine/base/DirectXBase.h"
#include "engine/math/func/Math.h"
#include "ModelManager.h"
#include <fstream>
#include <sstream>
#include <cassert>
//初期化
void Object3d::Initialize(Object3dCommon* object3dCommon){
	//Nullチェック
	assert(object3dCommon);
	//引数を受け取ってメンバ変数に記録
	object3dCommon_ = object3dCommon;
	//DirectXの基盤部分を受け取る
	directXBase_ = object3dCommon_->GetDirectXBase();
	//座標変換行列の生成
	CreateTransformationMatrixData();
	//光源の生成
	CreateDirectionLight();
	//Transform変数を作る
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.3f,0.0f,0.0f},{0.0f,4.0f,-10.0f} };
}

//更新
void Object3d::Update(){
	//ワールドマトリックスの生成
	Matrix4x4 worldMatrix = Rendering::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	//カメラワールドマトリックスの生成
	Matrix4x4 cameraWorldMatrix = Rendering::MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	//viewマトリックスの生成
	Matrix4x4 viewMatrix = ~cameraWorldMatrix;
	//透視投影行列を生成
	Matrix4x4 projectionMatrix = Rendering::MakePerspectiveFovMatrix(0.45f, float(WinApi::kClientWidth) / float(WinApi::kClientHeight), 0.1f, 100.0f);
	//wvpの書き込み
	wvpData_->WVP = worldMatrix * viewMatrix * projectionMatrix;
	//worldTransformの書き込み
	wvpData_->World = worldMatrix;
	if (model_) {
		model_->UVTransform(uvTransform_);
	}
}

//描画
void Object3d::Draw(){
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
void Object3d::SetModel(const std::string& filePath){
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}

void Object3d::SetModel(Model* model){
	model_ = model;
}

// スケールのセッター
void Object3d::SetScale(const Vector3& scale){
	transform_.scale = scale;
}

// 回転のセッター
void Object3d::SetRotate(const Vector3& rotate){
	transform_.rotate = rotate;
}

// 平行移動のセッター
void Object3d::SetTranslate(const Vector3& translate){
	transform_.translate = translate;
}

// uvスケールのセッター
void Object3d::SetUVScale(const Vector2& uvScale){
	uvTransform_.scale = uvScale;
}

// uv回転のセッター
void Object3d::SetUVRotate(const float uvRotate){
	uvTransform_.rotate = uvRotate;
}

// uv平行移動のセッター
void Object3d::SetUVTranslate(const Vector2& uvTranslate){
	uvTransform_.translate = uvTranslate;
}

// スケールのゲッター
const Vector3& Object3d::GetScale() const{
	// TODO: return ステートメントをここに挿入します
	return transform_.scale;
}

// 回転のゲッター
const Vector3& Object3d::GetRotate() const{
	// TODO: return ステートメントをここに挿入します
	return transform_.rotate;
}

// 平行移動のゲッター
const Vector3& Object3d::GetTranslate() const{
	// TODO: return ステートメントをここに挿入します
	return transform_.translate;
}

// uvスケールのゲッター
const Vector2& Object3d::GetUVScale() const{
	// TODO: return ステートメントをここに挿入します
	return uvTransform_.scale;
}

// uv回転のゲッター
const float Object3d::GetUVRotate() const{
	// TODO: return ステートメントをここに挿入します
	return uvTransform_.rotate;
}

// uv平行移動のゲッター
const Vector2& Object3d::GetUVTranslate() const{
	// TODO: return ステートメントをここに挿入します
	return uvTransform_.translate;
}

// 座標変換行列データの生成
void Object3d::CreateTransformationMatrixData(){
	//WVP用のリソースを作る
	wvpResource_ = directXBase_->CreateBufferResource(sizeof(TransformationMatrix));
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	//単位行列を書き込んでおく
	wvpData_->WVP = Math::MakeIdentity4x4();
	wvpData_->World = Math::MakeIdentity4x4();
}

//光源の生成
void Object3d::CreateDirectionLight(){
	//光源のリソースを作成
	directionalLightResource_ = directXBase_->CreateBufferResource(sizeof(DirectionalLight));
	//光源データの書きこみ
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;
}
