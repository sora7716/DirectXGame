#include "WorldTransform.h"
#include "engine/base/DirectXBase.h"
#include "engine/math/func/Rendering.h"
#include "engine/math/func/Math.h"
#include "engine/camera/Camera.h"
//メンバ関数テーブルの初期化
void(WorldTransform::* WorldTransform::UpdateTransformTable[])() = {
	&UpdateTransform,
	&UpdateTransform2d,
};

//初期化
void WorldTransform::Initialize(DirectXBase* directXBase, TransformMode transformMode) {
	//DirectXの基盤部分の記録
	directXBase_ = directXBase;
	//ワールド座標
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	//wvpリソースの初期化
	CreateTransformationMatrixResorce();
	transformMode_ = transformMode;
}

//更新
void WorldTransform::Update() {
	//トランスフォームの更新
	(this->*UpdateTransformTable[static_cast<uint32_t>(transformMode_)])();
}

//描画
void WorldTransform::Draw() {
	//座標変換行列CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());//wvp
}

//スクリーンに表示する範囲
void WorldTransform::SetScreenArea(ScreenArea screenArea){
	screenArea_ = screenArea;
}

//カメラのセッター
void WorldTransform::SetCamera(Camera* camera){
	camera_ = camera;
}

//親のセッター
void WorldTransform::SetParent(const WorldTransform* parent){
	parent_ = parent;
}

//ワールド座標のセッター
void WorldTransform::SetTransform(const Transform& transform){
	transform_ = transform;
}

//ワールド座標のセッター(2D)
void WorldTransform::SetTransform2d(const Transform2d& transform2d){
	transform_ = { 
		{transform2d.scale.x,transform2d.scale.y,1.0f},
		{0.0f,0.0f,transform2d.rotate},
		{transform2d.translate.x,transform2d.translate.y,0.0f} 
	};
}

//スケールのセッター
void WorldTransform::SetScale(const Vector3& scale){
	transform_.scale = scale;
}

//回転のセッター
void WorldTransform::SetRotate(const Vector3& rotate){
	transform_.rotate = rotate;
}

//平行移動のセッター
void WorldTransform::SetTranslate(const Vector3& translate){
	transform_.translate = translate;
}

//ワールド行列のゲッター
const Matrix4x4& WorldTransform::GetWorldMatrix() const{
	// TODO: return ステートメントをここに挿入します
	return worldMatrix_;
}

//スケールのゲッター
const Vector3& WorldTransform::GetScale() const{
	// TODO: return ステートメントをここに挿入します
	return transform_.scale;
}

//回転のゲッター
const Vector3& WorldTransform::GetRotate() const{
	// TODO: return ステートメントをここに挿入します
	return transform_.rotate;
}

//平行移動のセッター
const Vector3& WorldTransform::GetTranslate() const{
	// TODO: return ステートメントをここに挿入します
	return transform_.translate;
}

//座標変換行列リソースの生成
void WorldTransform::CreateTransformationMatrixResorce() {
	//座標変換行列リソースを作成する
	wvpResource_ = directXBase_->CreateBufferResource(sizeof(TransformationMatrix));
	//座標変換行列リソースにデータを書き込むためのアドレスを取得してtransformationMatrixDataに割り当てる
	//書き込むためのアドレス
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	//単位行列を書き込んでおく
	wvpData_->WVP = Math::MakeIdentity4x4();
	wvpData_->World = Math::MakeIdentity4x4();
}

//座標の更新
void WorldTransform::UpdateTransform() {
	//TransformからWorldMatrixを作る
	worldMatrix_ = Rendering::MakeAffineMatrix(transform_);
	if (parent_) {
		worldMatrix_ = worldMatrix_ * parent_->worldMatrix_;
	}
	//wvpの書き込み
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		wvpData_->WVP = worldMatrix_ * viewProjectionMatrix;
	}
	else {
		wvpData_->WVP = worldMatrix_;
	}
	//ワールド行列を送信
	wvpData_->World = worldMatrix_;
}

//座標の更新(2次元)
void WorldTransform::UpdateTransform2d(){
	//TransformからWorldMatrixを作る
	worldMatrix_ = Rendering::MakeAffineMatrix(transform_);
	if (parent_) {
		worldMatrix_ = worldMatrix_ * parent_->worldMatrix_;
	}
	//ProjectionMatrixを作って平行投影行列を書き込む
	const Matrix4x4& projectionMatrix = Rendering::MakeOrthographicMatrix(screenArea_.left, screenArea_.top, screenArea_.right, screenArea_.bottom, camera_->GetNearClip(), camera_->GetFarClip());
	//wvpの書き込み
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewMatrix() * projectionMatrix;
		wvpData_->WVP = worldMatrix_ * viewProjectionMatrix;
	}
	else {
		wvpData_->WVP = worldMatrix_ * projectionMatrix;
	}
	//ワールド行列を送信
	wvpData_->World = worldMatrix_;
}


