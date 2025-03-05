#include "Sprite.h"
#include "engine/objectManager/SpriteManager.h"
#include <cassert>
#include "engine/math/func/Math.h"
#include "TextureManager.h"
#include "engine/base/DirectXBase.h"

//初期化
void Sprite::Initialize(SpriteManager* spriteManager, std::string textureFilePath) {
	assert(spriteManager);//Nullチェック
	spriteManager_ = spriteManager;//共通部分を受け取る
	directXBase_ = spriteManager_->GetDirectXBase();//DirectXの基盤部分を受け取る
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
	InitialilzeVertexData();

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

	//光源
	directionalLightResource_ = directXBase_->CreateBufferResource(sizeof(DirectionalLight));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;

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

	//uvTransformの情報の初期化
	uvTransform_ = {
		.scale = {1.0f,1.0f,1.0f},
		.rotate = {},
		.translate{}
	};
	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);
}

//更新処理
void Sprite::Update() {
	//頂点リソースにデータを書き込む(4点分)
	//インデックスリソースにデータを書き込む(6個分)
	
	//メンバ変数の値を見た目に反映
	transform_.translate = { transform2D_.translation.x,transform2D_.translation.y,0.0f };
	transform_.rotate = { 0.0f,0.0f,transform2D_.rotation};
	transform_.scale = { transform2D_.size.x,transform2D_.size.y,1.0f };
	//メンバ変数の値を見た目に反映UV
	uvTransform_.translate = {uvTransform2D_.translation.x,uvTransform2D_.translation.y,0.0f };
	uvTransform_.rotate = { 0.0f,0.0f,uvTransform2D_.rotation };
	uvTransform_.scale = { uvTransform2D_.size.x,uvTransform2D_.size.y,1.0f };
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
	directXBase_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureIndex));
	//描画(DrwaCall/ドローコール)
	directXBase_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

//テクスチャの変更
void Sprite::ChangeTexture(std::string textureFilePath){
	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);
}

//サイズのゲッター
const Vector2& Sprite::GetSize() const{
	// TODO: return ステートメントをここに挿入します
	return transform2D_.size;
}

//回転のゲッター
float Sprite::GetRotation() const {
	return transform2D_.rotation;
}

// 位置のゲッター
const Vector2& Sprite::GetPosition() const {
	// TODO: return ステートメントをここに挿入します
	return transform2D_.translation;
}

//UVのサイズのゲッター
const Vector2& Sprite::GetUVSize() const{
	// TODO: return ステートメントをここに挿入します
	return uvTransform2D_.size;
}

//UVの回転のセッター
float Sprite::GetUVRotation() const{
	return uvTransform2D_.rotation;
}

//UVの位置のゲッター
const Vector2& Sprite::GetUVPosition() const{
	// TODO: return ステートメントをここに挿入します
	return uvTransform2D_.translation;
}

//色のゲッター
const Vector4& Sprite::GetColor() const{
	// TODO: return ステートメントをここに挿入します
	return materialData_->color;
}

//サイズのセッター
void Sprite::SetSize(const Vector2& size){
	transform2D_.size = size;
}

//回転のセッター
void Sprite::SetRotation(float rotation) {
	transform2D_.rotation= rotation;
}

// 位置のセッター
void Sprite::SetPosition(const Vector2& position) {
	transform2D_.translation = position;
}

//UVのサイズのセッター
void Sprite::SetUVSize(const Vector2& size){
	uvTransform2D_.size = size;
}

//UVの回転のセッター
void Sprite::SetUVRoation(float rotation){
	uvTransform2D_.rotation = rotation;
}

//UVの位置のセッター
void Sprite::SetUVPosition(const Vector2& position){
	uvTransform2D_.translation = position;
}

//色のセッター
void Sprite::SetColor(const Vector4& color){
	materialData_->color = color;
}

//頂点データの初期化
void Sprite::InitialilzeVertexData() {
	//1枚目の三角形
	vertexData_[0].position = { 0.0f,1.0f,0.0f,1.0f };//左下
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[0].normal = {0.0f,0.0f,-1.0f};

	vertexData_[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[1].normal = {0.0f,0.0f,-1.0f};

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
	materialData_->uvTransform = Rendering::MakeUVAffineMatrix(uvTransform_.scale, uvTransform_.rotate.z, uvTransform_.translate);
}
