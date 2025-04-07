#include "WorldTransform.h"
#include "engine/base/WinApi.h"
#include "engine/base/DirectXBase.h"
#include "engine/math/func/Math.h"
#include "engine/gameObject/Camera.h"

//初期化
void WorldTransform::Initialize(DirectXBase* directXBase) {
	//DirectXの基盤部分の記録
	directXBase_ = directXBase;
	//ワールド座標
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	//wvpリソースの初期化
	CreateTransformationMatrixResorce();
}

//更新
void WorldTransform::Update() {
	//トランスフォームの更新
	UpdateTransform2d();
}

//描画
void WorldTransform::Draw() {
	//座標変換行列CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());//wvp
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

//ローカル座標の更新
void WorldTransform::UpdateTransform2d() {
	//TransformからWorldMatrixを作る
	worldMatrix_ = Rendering::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
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


