#include "ParticleEmit.h"
#include "engine/base/DirectXBase.h"
#include "engine/objectCommon/ParticleCommon.h"
#include "engine/gameObject/Camera.h"
#include "engine/debug/ImGuiManager.h"
#include "engine/2d/TextureManager.h"
#include "engine/math/func/Math.h"
#include "engine/base/SRVManager.h"

//初期化
void ParticleEmit::Initialize(DirectXBase* directXBase) {
	//DirectXの基盤部分を記録する
	directXBase_ = directXBase;
	//ライトを生成
	ParticleCommon::GetInstance()->CreateDirectionLight();
	//トランスフォームの初期化
	transform_ = {
		.scale = {1.0f,1.0f,1.0f},
		.rotate = {0.0f,180.0f * rad,0.0f},
		.translate = {0.0f,0.0f,0.0f}
	};
	//ワールドトランスフォームのリソースの生成
	CreateWorldTransformResource();
	//カメラを設定
	camera_ = ParticleCommon::GetInstance()->GetDefaultCamera();
	//頂点リソースの生成
	CreateVertexResource();
	//マテリアルリソースの生成
	CreateMaterialResource();
	//テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
	particleResouce_ = directXBase_->CreateBufferResource(sizeof(TransformationMatrix) * kNumInstanceCount);
	SRVManager::GetInstance()->CreateSRVforStructuredBuffer(3, particleResouce_.Get(), kNumInstanceCount, sizeof(TransformationMatrix));
}

//更新
void ParticleEmit::Update() {
	//ワールドトランスフォームの更新
	UpdateWorldTransform();
#ifdef USE_IMGUI
	ImGui::Begin("parthicleEmit");
	ImGui::DragFloat3("scale", &transform_.scale.x, 0.1f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::End();
#endif // USE_IMGUI

}

//描画
void ParticleEmit::Draw() {
	//PSOの設定
	auto pso = ParticleCommon::GetInstance()->GetGraphicsPipelineStates()[static_cast<int32_t>(blendMode_)].Get();
	//グラフィックスパイプラインをセットするコマンド
	directXBase_->GetCommandList()->SetPipelineState(pso);
	//平光源CBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(3, ParticleCommon::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
	//VertexBufferViewの設定
	directXBase_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
	//マテリアルCBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());//material
	//SRVのDescriptorTableの先頭を設定
	directXBase_->GetCommandList()->SetGraphicsRootDescriptorTable(1, SRVManager::GetInstance()->GetGPUDescriptorHandle(3));
	//ワールドトランスフォームの描画
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());//wvp
	//描画(DrwaCall/ドローコール)
	directXBase_->GetCommandList()->DrawInstanced(static_cast<UINT>(modelData_.vertices.size()), kNumInstanceCount, 0, 0);
}

//終了
void ParticleEmit::Finalize() {

}

//モデルデータの初期化
void ParticleEmit::InitializeModelData() {
	modelData_.vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//左上
	modelData_.vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData_.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左下
	modelData_.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左下
	modelData_.vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData_.vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f},.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//右下
	modelData_.material.textureFilePath = "engine/resources/textures/uvChecker.png";
}

//マテリアルデータの初期化
void ParticleEmit::InitializeMaterialData() {
	//色を書き込む
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->enableLighting = false;
	materialData_->uvTransform = Math::MakeIdentity4x4();
}

//頂点リソースの生成
void ParticleEmit::CreateVertexResource() {
	//頂点データの初期化
	InitializeModelData();
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

//マテリアルリソースの生成
void ParticleEmit::CreateMaterialResource() {
	//マテリアルリソースを作る
	materialResource_ = directXBase_->CreateBufferResource(sizeof(Material));
	//マテリアルリソースにデータを書き込むためのアドレスを取得してmaterialDataに割り当てる
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//マテリアルデータの初期値を書き込む
	InitializeMaterialData();
}

//ワールドトランスフォームのリソースの生成
void ParticleEmit::CreateWorldTransformResource() {
	//座標変換行列リソースを作成する
	wvpResource_ = directXBase_->CreateBufferResource(sizeof(TransformationMatrix) * kNumInstanceCount);
	//座標変換行列リソースにデータを書き込むためのアドレスを取得してtransformationMatrixDataに割り当てる
	//書き込むためのアドレス
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&instanceDatas_));
	for (uint32_t i = 0;i < kNumInstanceCount;i++) {
		//単位行列を書き込んでおく
		instanceDatas_[i].WVP = Math::MakeIdentity4x4();
		instanceDatas_[i].World = Math::MakeIdentity4x4();
	}
}

//ワールドトランスフォームの更新
void ParticleEmit::UpdateWorldTransform() {
	for (uint32_t i = 0;i < kNumInstanceCount;i++) {
		//TransformからWorldMatrixを作る
		worldMatrix_ = Rendering::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
		/*if (parent_) {
			worldMatrix_ = worldMatrix_ * parent_->worldMatrix_;
		}*/
		//wvpの書き込み
		if (camera_) {
			const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
			instanceDatas_[i].WVP = worldMatrix_ * viewProjectionMatrix;
		} else {
			instanceDatas_[i].WVP = worldMatrix_;
		}
		//ワールド行列を送信
		instanceDatas_[i].World = worldMatrix_;
	}
}
