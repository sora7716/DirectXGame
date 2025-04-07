#include "WorldTransformPlane.h"
#include "engine/base/WinApi.h"
#include "engine/base/DirectXBase.h"
#include "engine/math/func/Math.h"
#include "engine/gameObject/Camera.h"

//初期化
void WorldTransformPlane::Initialize(DirectXBase* directXBase) {
	//DirectXの基盤部分の記録
	directXBase_ = directXBase;
	//ワールド座標
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	//ワールド2D座標の初期化
	transform2d_ = { {1.0f,1.0f},0.0f,{0.0f,0.0f} };
	//wvpリソースの初期化
	CreateTransformationMatrixResorce();
}

//更新
void WorldTransformPlane::Update() {
	//2Dに直す
	transform_.scale = { transform2d_.scale.x,transform2d_.scale.y,1.0f };
	transform_.rotate = { 0.0f,0.0f,transform2d_.rotate };
	transform_.translate = { transform2d_.translate.x,transform2d_.translate.y,0.0f };
	//トランスフォームの更新
	UpdateTransform2d();
}

//描画
void WorldTransformPlane::Draw() {
	//座標変換行列CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());//wvp
}

//座標変換行列リソースの生成
void WorldTransformPlane::CreateTransformationMatrixResorce() {
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
void WorldTransformPlane::UpdateTransform2d() {
	//TransformからWorldMatrixを作る
	worldMatrix_ = Rendering::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	if (parent_) {
		worldMatrix_ = worldMatrix_ * parent_->worldMatrix_;
	}
	//画面を切り取るところ
	PlanePoint vertex = {
		-(float)WinApi::kClientWidth / 2.0f,
		(float)WinApi::kClientHeight / 2.0f,
		(float)WinApi::kClientWidth / 2.0f,
		-(float)WinApi::kClientHeight / 2.0f
	};
	//ProjectionMatrixを作って平行投影行列を書き込む
	const Matrix4x4& projectionMatrix = Rendering::MakeOrthographicMatrix(vertex.left, vertex.top, vertex.right, vertex.bottom, 0.0f, 100.0f);
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


