#include "ParticleManager.h"
#include "engine/base/DirectXBase.h"
#include "engine/base/SRVManager.h"
#include "engine/objectCommon/BaseObjectCommon.h"
#include <cassert>

//インスタンスのゲッター
ParticleManager* ParticleManager::GetInstance(){
    assert(!isFinalize && "GetInstance() called after Finalize()");
    if (instance == nullptr) {
        instance = new ParticleManager();
    }
    return instance;
}

//初期化
void ParticleManager::Initialize(DirectXBase* directXBase){
    //DirectXの基盤部分を記録する
    directXBase_ = directXBase;
    //オブジェクトの共通部分の基底クラスの生成
    baseObjectCommon_ = std::make_unique<BaseObjectCommon>();
    //パイプラインの生成
    baseObjectCommon_->Initialize(directXBase_);
	//頂点データと頂点リソースの生成
	CreateVertexResorce();
	//インデックスデータとインデックスリソースの生成
	CreateIndexResorce();
}

//頂点データの初期化
void ParticleManager::InitializeVertexData() {
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
void ParticleManager::CreateVertexResorce() {
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
	//頂点データの初期化
	InitializeVertexData();
}

//インデックスデータの初期化
void ParticleManager::InitializeIndexData() {
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 1; indexData_[4] = 4; indexData_[5] = 2;
}

//インデックスリソースの生成
void ParticleManager::CreateIndexResorce() {
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
	//インデックスリソースの初期化
	InitializeIndexData();
}

//終了
void ParticleManager::Finalize(){
    delete instance;
    instance = nullptr;
    baseObjectCommon_.reset();
    baseObjectCommon_ = nullptr;
    isFinalize = true;
}
