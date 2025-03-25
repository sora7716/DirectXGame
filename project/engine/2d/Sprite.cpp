#include "Sprite.h"
#include "engine/objectCommon/SpriteCommon.h"
#include <cassert>
#include "engine/math/func/Math.h"
#include "TextureManager.h"
#include "engine/base/DirectXBase.h"
#include "WinApi.h"

//初期化
void Sprite::Initialize(SpriteCommon* spriteCommon, std::string textureFilePath) {
	assert(spriteCommon);//Nullチェック
	spriteCommon_ = spriteCommon;//共通部分を受け取る
	directXBase_ = spriteCommon_->GetDirectXBase();//DirectXの基盤部分を受け取る
	//頂点データの生成
	CreateVertexResorce();
	//インデックスデータの生成
	CreateIndexResorce();
	//マテリアルデータの生成
	CreateMaterialResorce();
	//座標変換行列データの生成
	CreateTransformationMatrixResorce();
	//光源の生成
	CreateDirectionLight();

	//Transformの情報の初期化
	transform_ = {
		.scale = {1.0f,1.0f,1.0f},
		.rotate = {},
		.translate{}
	};

	//uvTransformの情報の初期化
	uvTransform_ = {
		.scale = {1.0f,1.0f},
		.rotate = {},
		.translate{}
	};
	TextureManager::GetInstance()->LoadTexture(textureFilePath);
	filePath_ = textureFilePath;
}

//更新処理
void Sprite::Update() {
	//頂点リソースにデータを書き込む(4点分)
	//インデックスリソースにデータを書き込む(6個分)

	//メンバ変数の値を見た目に反映
	transform_.scale = { transform2D_.scale.x,transform2D_.scale.y,1.0f };
	transform_.rotate = { 0.0f,0.0f,transform2D_.rotate };
	transform_.translate = { transform2D_.translate.x,transform2D_.translate.y,0.0f };
	//Transform情報を作成する
	UpdateTransform();
	//UVTransform情報を作成する
	UpdateUVTransform();
}

//描画処理
void Sprite::Draw() {
	//VertexBufferViewの設定
	directXBase_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
	//IndexBufferViewを設定
	directXBase_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);//IBVを設定

	//マテリアルCBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());//material
	//座標変換行列CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResoruce_->GetGPUVirtualAddress());//wvp
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	//SRVのDescriptorTableの先頭を設定
	directXBase_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVHandleGPU(filePath_));
	//描画(DrwaCall/ドローコール)
	directXBase_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

//テクスチャの変更
void Sprite::ChangeTexture(std::string textureFilePath) {
	filePath_ = textureFilePath;
}

//サイズのゲッター
const Vector2& Sprite::GetScale() const {
	// TODO: return ステートメントをここに挿入します
	return transform2D_.scale;
}

//回転のゲッター
float Sprite::GetRotate() const {
	return transform2D_.rotate;
}

// 位置のゲッター
const Vector2& Sprite::GetTranslate() const {
	// TODO: return ステートメントをここに挿入します
	return transform2D_.translate;
}

//UVのサイズのゲッター
const Vector2& Sprite::GetUVScale() const {
	// TODO: return ステートメントをここに挿入します
	return uvTransform_.scale;
}

//UVの回転のセッター
float Sprite::GetUVRotate() const {
	return uvTransform_.rotate;
}

//UVの位置のゲッター
const Vector2& Sprite::GetUVTranslate() const {
	// TODO: return ステートメントをここに挿入します
	return uvTransform_.translate;
}

//色のゲッター
const Vector4& Sprite::GetColor() const {
	// TODO: return ステートメントをここに挿入します
	return materialData_->color;
}

//サイズのセッター
void Sprite::SetScale(const Vector2& scale) {
	transform2D_.scale = scale;
}

//回転のセッター
void Sprite::SetRotate(float rotate) {
	transform2D_.rotate = rotate;
}

// 位置のセッター
void Sprite::SetTranslate(const Vector2& translate) {
	transform2D_.translate = translate;
}

//UVのサイズのセッター
void Sprite::SetUVScale(const Vector2& scale) {
	uvTransform_.scale = scale;
}

//UVの回転のセッター
void Sprite::SetUVRotate(float rotate) {
	uvTransform_.rotate = rotate;
}

//UVの位置のセッター
void Sprite::SetUVTranslate(const Vector2& translate) {
	uvTransform_.translate = translate;
}

//色のセッター
void Sprite::SetColor(const Vector4& color) {
	materialData_->color = color;
}

//頂点データの初期化
void Sprite::InitializeVertexData() {
	//1枚目の三角形
	vertexData_[0].position = { 0.0f,1.0f,0.0f,1.0f };//左下
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[0].normal = { 0.0f,0.0f,-1.0f };

	vertexData_[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[1].normal = { 0.0f,0.0f,-1.0f };

	vertexData_[2].position = { 1.0f,1.0f,0.0f,1.0f };//右下
	vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[2].normal = { 0.0f,0.0f,-1.0f };

	//2枚目の三角形
	vertexData_[3].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexData_[3].texcoord = { 0.0f,0.0f };
	vertexData_[3].normal = { 0.0f,0.0f,-1.0f };
	vertexData_[4].position = { 1.0f,0.0f,0.0f,1.0f };//右上
	vertexData_[4].texcoord = { 1.0f,0.0f };
	vertexData_[4].normal = { 0.0f,0.0f,-1.0f };
	vertexData_[5].position = { 1.0f,1.0f,0.0f,1.0f };//右下
	vertexData_[5].texcoord = { 1.0f,1.0f };
	vertexData_[5].normal = { 0.0f,0.0f,-1.0f };
}

//頂点データの生成
void Sprite::CreateVertexResorce() {
	//VertexResourceを作成する
	vertexResource_ = directXBase_->CreateBufferResource(sizeof(VertexData) * 6);
	//VertexBufferViewを作成する
	//リソースの先頭アドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//VertexResorceにデータを書き込むためのアドレスを取得してvertexDataに割り当てる
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	InitializeVertexData();
}

//インデックスデータの初期化
void Sprite::InitializeIndexData() {
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 1; indexData_[4] = 4; indexData_[5] = 2;
}

//インデックスリソースの生成
void Sprite::CreateIndexResorce() {
	//IndexResourceを作成する
	indexResource_ = directXBase_->CreateBufferResource(sizeof(uint32_t) * 6);
	//IndexBufferViewを作成する
	//リソースの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//IndexResourceにデータを書き込むためのアドレスを取得してindexDataに割り当てる
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	InitializeIndexData();
}

//マテリアルデータの初期化
void Sprite::InitializeMaterialData() {
	//色を書き込む
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->enableLighting = false;
	materialData_->uvTransform = Math::MakeIdentity4x4();
}

//マテリアルリソースの生成
void Sprite::CreateMaterialResorce() {
	//マテリアルリソースを作る
	materialResource_ = directXBase_->CreateBufferResource(sizeof(Material));
	//マテリアルリソースにデータを書き込むためのアドレスを取得してmaterialDataに割り当てる
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//マテリアルデータの初期値を書き込む
	InitializeMaterialData();
}

//座標変換行列データの初期化
void Sprite::InitializeTransformationMatrixData() {
	//単位行列を書き込んでおく
	transformationMatrixData_->WVP = Math::MakeIdentity4x4();
	transformationMatrixData_->World = Math::MakeIdentity4x4();
}

//座標変換行列リソースの生成
void Sprite::CreateTransformationMatrixResorce() {
	//座標変換行列リソースを作成する
	transformationMatrixResoruce_ = directXBase_->CreateBufferResource(sizeof(TransformationMatrix));
	//座標変換行列リソースにデータを書き込むためのアドレスを取得してtransformationMatrixDataに割り当てる
	//書き込むためのアドレス
	transformationMatrixResoruce_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	InitializeTransformationMatrixData();
}

//光源の生成
void Sprite::CreateDirectionLight() {
	//光源
	directionalLightResource_ = directXBase_->CreateBufferResource(sizeof(DirectionalLight));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;
}

// 座標変換の更新
void Sprite::UpdateTransform() {
	//TransformからWorldMatrixを作る
	transformationMatrixData_->World = Rendering::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	//ViewMatirxを作って単位行列を入れる
	Matrix4x4 viewMatrix = Math::MakeIdentity4x4();
	//ProjectionMatrixを作って平行投影行列を書き込む
	Matrix4x4 projectionMatrix = Rendering::MakeOrthographicMatrix(0.0f, 0.0f, (float)WinApi::kClientWidth, (float)WinApi::kClientHeight, 0.0f, 100.0f);
	transformationMatrixData_->WVP = transformationMatrixData_->World * (viewMatrix * projectionMatrix);
}

// UVの座標変換の更新
void Sprite::UpdateUVTransform() {
	//UVTransform
	materialData_->uvTransform = Rendering::MakeUVAffineMatrix({ uvTransform_.scale.x,uvTransform_.scale.y,1.0f }, uvTransform_.rotate, { uvTransform_.translate.x,uvTransform_.translate.y,1.0f });
}
