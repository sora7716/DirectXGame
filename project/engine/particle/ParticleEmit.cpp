#include "ParticleEmit.h"
#include "engine/base/DirectXBase.h"
#include "engine/math/func/Math.h"
#include "engine/2d/TextureManager.h"

//初期化
void ParticleEmit::Initialize(DirectXBase* directXBase){
	//DirectXの基盤部分を記録する
	directXBase_ = directXBase;
	//モデルデータの初期化
	InitializeModelDate();
	//頂点リソースの生成
	CreateVertexResource();
	//インデックスリソースの生成
	CreateIndexResource();
	//マテリアルリソースの生成
	CreateMaterialResource();
	//ワールドトランスフォーム
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize(directXBase_,TransformMode::k3d);
	worldTransform_->SetScale({ 1000.0f,1000.0f,1.0f });
}

//更新
void ParticleEmit::Update(){
	//ワールドトランスフォーム
	worldTransform_->Update();
}

//描画
void ParticleEmit::Draw(){
	//ワールドトランスフォーム
	worldTransform_->Draw();
	//VertexBufferViewの設定
	directXBase_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
	directXBase_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);//IBVを設定
	//マテリアルCBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定
	directXBase_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVHandleGPU(modelData_.material.textureFilePath));
	directXBase_->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 100, 0, 0);
}

/// モデルデータの初期化
void ParticleEmit::InitializeModelDate(){
	modelData_.vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });
	modelData_.vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });
	modelData_.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });
	modelData_.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });
	modelData_.vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });
	modelData_.vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f},.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });
	modelData_.material.textureFilePath = "engine/resources/textures/uvChecker.png";
}

//頂点リソースの生成
void ParticleEmit::CreateVertexResource(){
	//頂点リソースを生成
	vertexResource_ = directXBase_->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	//VertexBufferViewを作成する(頂点バッファービュー)
	//リソースの先頭アドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());//頂点データをリソースにコピー
}

//インデックスリソースの生成
void ParticleEmit::CreateIndexResource(){
	//Index用(3dGameObject)
	indexResource_ = directXBase_->CreateBufferResource(sizeof(uint32_t) * modelData_.vertices.size());
	//リソースの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * modelData_.vertices.size());
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	//IndexResourceにデータを書き込む
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	for (int i = 0; i < modelData_.vertices.size(); i++) {
		indexData_[i] = i; indexData_[i + 1] = i + 1; indexData_[i + 2] = i + 2;
		indexData_[i + 3] = i + 1; indexData_[i + 4] = i + 3; indexData_[i + 5] = i + 2;
	}
}

//マテリアルリソースの生成
void ParticleEmit::CreateMaterialResource(){
	//マテリアル用のリソースを作る
	materialResource_ = directXBase_->CreateBufferResource(sizeof(Material));
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//色を書き込む
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->enableLighting = true;
	materialData_->uvTransform = Math::MakeIdentity4x4();
}
