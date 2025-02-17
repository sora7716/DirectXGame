#include "Sprite.h"
#include "SpriteGeneral.h"
#include <cassert>
#include "engine/math/func/Math.h"

//初期化
void Sprite::Initialize(SpriteGeneral* spriteGeneral) {
	assert(spriteGeneral);//Nullチェック
	spriteGeneral_ = spriteGeneral;//共通部分を受け取る
	directXBase_ = spriteGeneral_->GetDirectXBase();//DirectXの基盤部分を受け取る
	//VertexResourceを作成する
	vertexResource_ = directXBase_->CreateBufferResource(sizeof(VertexData) * 6);
	//IndexResourceを作成する
	indexResource_ = directXBase_->CreateBufferResource(sizeof(uint32_t) * 6);
	//VertexBufferViewを作成する
	//リソースの先頭アドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	//IndexBufferViewを作成する
	//リソースの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	//VertexResorceにデータを書き込むためのアドレスを取得してvertexDataに割り当てる
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//IndexResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	InitializeIndexData();
	//マテリアルリソースを作る
	materialResource_ = directXBase_->CreateBufferResource(sizeof(Material));
	//マテリアルリソースにデータを書き込むためのアドレスを取得してmaterialDataに割り当てる
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//マテリアルデータの初期値を書き込む
	InitialilzeMaterialData();
	//座標変換行列リソースを作成する
	transformationMatrixResoruce_ = directXBase_->CreateBufferResource(sizeof(TransformationMatrix));
	//座標変換行列リソースにデータを書き込むためのアドレスを取得してtransformationMatrixDataに割り当てる
	//書き込むためのアドレス
	transformationMatrixResoruce_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	InitializeTransformationMatrixData();

	//Transformの情報の初期化
	transform_ = {
		.scale = {1.0f,1.0f,1.0f},
		.rotate = {},
		.translate{}
	};
}

//更新処理
void Sprite::Update() {
	//頂点リソースにデータを書き込む(4点分)
	//インデックスリソースにデータを書き込む(6個分)
	ImGui::Begin("sprite");
	ImGui::DragFloat3("scale", &transform_.scale.x, 0.1f, 0.0f, 5.0f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::End();
	//Transform情報を作成する
	//TransformからWorldMatrixを作る
	transformationMatrixData_->World = Rendering::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	//ViewMatirxを作って単位行列を入れる
	Matrix4x4 viewMatrix = Math::MakeIdentity4x4();
	//ProjectionMatrixを作って平行投影行列を書き込む
	Matrix4x4 projectionMatrix = Rendering::MakeOrthographicMatrix(0.0f, 0.0f, (float)WinApi::kClientWidth, (float)WinApi::kClientHeight, 0.0f, 100.0f);
	transformationMatrixData_->WVP = transformationMatrixData_->World * (viewMatrix * projectionMatrix);
}

//描画処理
void Sprite::Draw(const D3D12_GPU_DESCRIPTOR_HANDLE& texture){
	//VertexBufferViewの設定
	directXBase_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
	//IndexBufferViewを設定
	directXBase_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);//IBVを設定

	//マテリアルCBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());//material
	//座標変換行列CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResoruce_->GetGPUVirtualAddress());//wvp

	//SRVのDescriptorTableの先頭を設定
	directXBase_->GetCommandList()->SetGraphicsRootDescriptorTable(2, texture);
	//描画(DrwaCall/ドローコール)
	directXBase_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

//頂点データの初期化
void Sprite::InitialilzeVertexData() {
	//1枚目の三角形
	vertexData_[0].position = { 0.0f,360.0f,0.0f,1.0f };//左下
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[0].normal = {};
	vertexData_[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[1].normal = {};
	vertexData_[2].position = { 640.0f,360.0f,0.0f,1.0f };//右下
	vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[2].normal = {};

	//2枚目の三角形
	vertexData_[3].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexData_[3].texcoord = { 0.0f,0.0f };
	vertexData_[3].normal = {};
	vertexData_[4].position = { 640.0f,0.0f,0.0f,1.0f };//右上
	vertexData_[4].texcoord = { 1.0f,0.0f };
	vertexData_[4].normal = {};
	vertexData_[5].position = { 640.0f,360.0f,0.0f,1.0f };//右下
	vertexData_[5].texcoord = { 1.0f,1.0f };
	vertexData_[5].normal = {};
}

//インデックスの初期化
void Sprite::InitializeIndexData() {
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 1; indexData_[4] = 4; indexData_[5] = 2;
}

//マテリアルデータの初期化
void Sprite::InitialilzeMaterialData() {
	//色を書き込む
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->enableLighting = false;
	materialData_->uvTransform = Math::MakeIdentity4x4();
}

//座標変換行列の初期化
void Sprite::InitializeTransformationMatrixData() {
	//単位行列を書き込んでおく
	transformationMatrixData_->WVP = Math::MakeIdentity4x4();
	transformationMatrixData_->World = Math::MakeIdentity4x4();
}
