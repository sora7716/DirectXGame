#include "Sprite.h"
#include "engine/2d/SpriteCommon.h"
#include "engine/2d/Object2dCommon.h"
#include <cassert>
#include "engine/math/func/Math.h"
#include "TextureManager.h"
#include "engine/base/DirectXBase.h"
#include "WinApi.h"
#include "engine/worldTransform/WorldTransform.h"
#include "engine/debug/ImGuiManager.h"

//デストラクタ
Sprite::~Sprite() {
	delete worldTransform_;
}

//初期化
void Sprite::Initialize(const std::string& spriteName) {
	directXBase_ = SpriteCommon::GetInstance()->GetDirectXBase();//DirectXの基盤部分を受け取る
	//頂点データの生成
	CreateVertexResource();
	//インデックスデータの生成
	CreateIndexResource();
	//マテリアルデータの生成
	CreateMaterialResource();
	//スプライトファイルパスを記録
	spriteName_ = "engine/resources/textures/" + spriteName;
	//スプライトの共通部分
	SpriteCommon::GetInstance()->LoadTexture(spriteName_);
	//DirectXの基盤部分を記録
	directXBase_ = SpriteCommon::GetInstance()->GetDirectXBase();
	//ライトの生成
	Object2dCommon::GetInstance()->CreateDirectionLight();
	//スクリーンに表示する範囲を設定
	WorldTransform::ScreenArea screenArea = {
		.left = 0,
		.top = 0,
		.right = (float)WinApi::kClientWidth,
		.bottom = (float)WinApi::kClientHeight,
	};
	//ワールドトランスフォーム
	worldTransform_ = new WorldTransform();
	worldTransform_->Initialize(directXBase_, TransformMode::k2d);
	worldTransform_->SetScreenArea(screenArea);
	worldTransform_->SetCamera(Object2dCommon::GetInstance()->GetDefaultCamera());
}

//更新
void Sprite::Update() {
#ifdef USE_IMGUI
	ImGui::Begin("sprite");
	ImGui::DragFloat3("scale", &transform_.scale.x, 0.1f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::End();
#endif // USE_IMGUI
	worldTransform_->SetTransform(transform_);
	//ワールドトランスフォームの更新
	worldTransform_->Update();
}

//描画処理
void Sprite::Draw() {
	//描画準備
	Object2dCommon::GetInstance()->DrawSetting();
	//PSOの設定
	auto pso = Object2dCommon::GetInstance()->GetGraphicsPipelineStates()[static_cast<int32_t>(blendMode_)].Get();
	//グラフィックスパイプラインをセットするコマンド
	directXBase_->GetCommandList()->SetPipelineState(pso);
	//ワールドトランスフォームの描画
	worldTransform_->Draw();
	//平光源CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(3, Object2dCommon::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
	//VertexBufferViewの設定
	directXBase_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
	//IndexBufferViewを設定
	directXBase_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);//IBVを設定
	//マテリアルCBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());//material
	//SRVのDescriptorTableの先頭を設定
	directXBase_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVHandleGPU(spriteName_));
	//描画(DrawCall/ドローコール)
	directXBase_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

//テクスチャの変更
void Sprite::ChangeTexture(std::string spriteName) {
	spriteName_ = spriteName;
}

// UVの座標変換の更新
void Sprite::UpdateUVTransform(Transform2d uvTransform) {
	//UVTransform
	materialData_->uvTransform = Rendering::MakeUVAffineMatrix({ uvTransform.scale.x,uvTransform.scale.y,1.0f }, uvTransform.rotate, { uvTransform.translate.x,uvTransform.translate.y,1.0f });
}

//色のゲッター
const Vector4& Sprite::GetColor() const {
	// TODO: return ステートメントをここに挿入します
	return materialData_->color;
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
void Sprite::CreateVertexResource() {
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
void Sprite::CreateIndexResource() {
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
void Sprite::CreateMaterialResource() {
	//マテリアルリソースを作る
	materialResource_ = directXBase_->CreateBufferResource(sizeof(Material));
	//マテリアルリソースにデータを書き込むためのアドレスを取得してmaterialDataに割り当てる
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//マテリアルデータの初期値を書き込む
	InitializeMaterialData();
}
