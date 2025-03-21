#include "ParticleManager.h"
#include <cassert>
#include "engine/base/DirectXBase.h"
#include "engine/base/SRVManager.h"
#include "engine/objectCommon/BaseObjectCommon.h"
#include "engine/2d/TextureManager.h"
#include "engine/3d/Camera.h"

// インスタンスのゲッター
ParticleManager* ParticleManager::GetInstance(){
	assert(!isFinalize && "GerInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new ParticleManager();
	}
	return instance;
}

//初期化
void ParticleManager::Initialize(DirectXBase* directXBase, SRVManager* srvManager, BaseObjectCommon* baseObjectCommon){
	//DirectXの基盤部分を記録する
	directXBase_ = directXBase;
	//SRVの管理を記録する
	srvManager_ = srvManager;
	//オブジェクトの共通部分を記録する
	baseObjectCommon_ = baseObjectCommon;
	//ランダムデバイスの生成
	std::mt19937 engine(randomDevice_());
	//パイプラインの生成
	graphicsPiplene_ = baseObjectCommon_->GetGraphicsPipelineState();
	//頂点データの生成
	CreateVertexResource();
	//インデックスリソースの生成
	CreateIndexResource();
}

//更新
void ParticleManager::Update(Camera* camera){
	
}

//終了
void ParticleManager::Finalize(){
	delete instance;
	instance = nullptr;
	isFinalize = true;
}

// 頂点データの初期化
void ParticleManager::InitializeVertexData(){
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

//インデックスデータの初期化
void ParticleManager::InitializeIndexData(){
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 1; indexData_[4] = 4; indexData_[5] = 2;
}

// 頂点リソースの生成
void ParticleManager::CreateVertexResource(){
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

//インデックスデータの生成
void ParticleManager::CreateIndexResource() {
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

// パーティクルグループの生成
void ParticleManager::CreateParticleGroup(const std::string& name, const std::string& textureFilePath){
	assert(particleGroups_.contains(name));
	ParticleGroup particleGroup;
	particleGroup.materialData.textureFilePath = textureFilePath;
	TextureManager::GetInstance()->LoadTexture(particleGroup.materialData.textureFilePath);
	particleGroup.materialData.srvIndex = TextureManager::GetInstance()->GetSRVIndex(particleGroup.materialData.textureFilePath);
	particleGroup.resource = directXBase_->CreateBufferResource(particleGroup.materialData.srvIndex);
	particleGroup.srvIndex = srvManager_->Allocate();
	srvManager_->CreateSRVforStructuredBuffer(particleGroup.srvIndex,particleGroup.resource.Get(),particleGroup.instanceCount,sizeof(InstanceData));
}
