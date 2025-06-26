#define NOMINMAX
#include <algorithm>
#include "ParticleSystem.h"
#include "engine/base/DirectXBase.h"
#include "engine/particle/ParticleCommon.h"
#include "engine/camera/Camera.h"
#include "engine/debug/ImGuiManager.h"
#include "engine/2d/TextureManager.h"
#include "engine/math/func/Math.h"
#include "engine/base/SRVManager.h"

//初期化
void ParticleSystem::Initialize(DirectXBase* directXBase, const std::string& textureName) {
	//DirectXの基盤部分を記録する
	directXBase_ = directXBase;
	//カメラを設定
	camera_ = ParticleCommon::GetInstance()->GetDefaultCamera();
	//ワールドトランスフォームのリソースの生成
	CreateWorldTransformResource();
	//頂点リソースの生成
	CreateVertexResource();
	//テクスチャファイルの記録
	modelData_.material.textureFilePath = "engine/resources/textures/" + textureName;
	//テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
	//マテリアルリソースの生成
	CreateMaterialResource();
	//インデックスリソースの生成
	CreateIndexResource();
	//ストラクチャバッファの生成
	CreateStructuredBuffer();

	//乱数エンジンの初期化
	std::random_device seedGenerator;
	randomEngine_.seed(seedGenerator());
}

//更新
void ParticleSystem::Update() {
	//生存しているパーティクルの数を0に初期化
	numInstance_ = 0;

	//更新処理
	for (auto it = particles_.begin(); it != particles_.end();) {
		if (numInstance_ < kNumMaxInstance) {
			if ((*it).lifeTime <= (*it).currentTime) {
				it = particles_.erase(it); //生存期間を過ぎたらパーティクルをlistから削除
				continue;//削除したので次のループへ
			}
			// パーティクルの色を設定
			instancingData_[numInstance_].color.SetRGB((*it).color.GetRGB());
			//Field内のParticleには加速度を適用する
			if (IsCollision(accelerationField_.area, (*it).transform.translate)) {
				(*it).velocity += accelerationField_.acceleration * Math::kDeltaTime;
			}
			//移動
			(*it).transform.translate += (*it).velocity * Math::kDeltaTime;
			//経過時間を足す
			(*it).currentTime += Math::kDeltaTime;
			float alpha = 1.0f - ((*it).currentTime / (*it).lifeTime);
			instancingData_[numInstance_].color.w = alpha;
			//ワールドトランスフォームの更新
			UpdateWorldTransform(numInstance_, it);
			//生きているパーティクルの数を記録
			numInstance_++;

		}
		//次のイテレータに進める
		it++;
	}

	//Emitterの更新
	emitter_.frequencyTime += Math::kDeltaTime;
	if (emitter_.frequency <= emitter_.frequencyTime) {
		particles_.splice(particles_.end(), Emit());
		emitter_.frequencyTime -= emitter_.frequency;//余計に過ぎた時間も加味して頻度を計算する
	}


#ifdef  USE_IMGUI
	ImGui::Begin("particle");
	ImGui::Text("size:%d", particles_.size());
	ImGui::Text("instance:%d", numInstance_);
	ImGui::DragFloat3("acceleration", &accelerationField_.acceleration.x, 0.1f);
	ImGui::DragFloat3("translate", &emitter_.transform.translate.x, 0.1f);
	ImGui::End();
#endif //USE_IMGUI
}

//描画
void ParticleSystem::Draw() {
	//描画準備
	ParticleCommon::GetInstance()->DrawSetting();
	//PSOの設定
	auto pso = ParticleCommon::GetInstance()->GetGraphicsPipelineStates()[static_cast<int32_t>(blendMode_)].Get();
	//グラフィックスパイプラインをセットするコマンド
	directXBase_->GetCommandList()->SetPipelineState(pso);
	//VertexBufferViewの設定
	directXBase_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
	//IndexBufferViewの設定
	directXBase_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);//IBVを設定
	//マテリアルCBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());//material
	//ワールドトランスフォームの描画
	directXBase_->GetCommandList()->SetGraphicsRootDescriptorTable(1, SRVManager::GetInstance()->GetGPUDescriptorHandle(srvIndex_));
	//SRVのDescriptorTableの先頭を設定
	directXBase_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVHandleGPU(modelData_.material.textureFilePath));
	//描画(DrawCall/ドローコール)
	directXBase_->GetCommandList()->DrawIndexedInstanced(static_cast<UINT>(modelData_.vertices.size()), numInstance_, 0, 0, 0);
}

//終了
void ParticleSystem::Finalize() {

}

std::string ParticleSystem::GetTextureName() {
	return modelData_.material.textureFilePath;
}

//モデルデータの初期化
void ParticleSystem::InitializeModelData() {
	modelData_.vertices.push_back({
		.position = {1.0f,1.0f,0.0f,1.0f},
		.texcoord = {0.0f,0.0f},
		.normal = {0.0f,0.0f,1.0f}
		});//左上
	modelData_.vertices.push_back({
		.position = {-1.0f,1.0f,0.0f,1.0f},
		.texcoord = {1.0f,0.0f},
		.normal = {0.0f,0.0f,1.0f}
		});//右上
	modelData_.vertices.push_back({
		.position = {1.0f,-1.0f,0.0f,1.0f},
		.texcoord = {0.0f,1.0f},
		.normal = {0.0f,0.0f,1.0f}
		});//左下
	modelData_.vertices.push_back({
		.position = {1.0f,-1.0f,0.0f,1.0f},
		.texcoord = {0.0f,1.0f},
		.normal = {0.0f,0.0f,1.0f}
		});//左下
	modelData_.vertices.push_back({
		.position = {-1.0f,1.0f,0.0f,1.0f},
		.texcoord = {1.0f,0.0f},
		.normal = {0.0f,0.0f,1.0f}
		});//右上
	modelData_.vertices.push_back({
		.position = {-1.0f,-1.0f,0.0f,1.0f},
		.texcoord = {1.0f,1.0f},
		.normal = {0.0f,0.0f,1.0f}
		});//右下
}

//マテリアルデータの初期化
void ParticleSystem::InitializeMaterialData() {
	//色を書き込む
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->enableLighting = false;
	materialData_->uvTransform = Math::MakeIdentity4x4();
}

//頂点リソースの生成
void ParticleSystem::CreateVertexResource() {
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
void ParticleSystem::CreateMaterialResource() {
	//マテリアルリソースを作る
	materialResource_ = directXBase_->CreateBufferResource(sizeof(Material));
	//マテリアルリソースにデータを書き込むためのアドレスを取得してmaterialDataに割り当てる
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//マテリアルデータの初期値を書き込む
	InitializeMaterialData();
}

void ParticleSystem::CreateIndexResource() {
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

//ワールドトランスフォームのリソースの生成
void ParticleSystem::CreateWorldTransformResource() {
	//座標変換行列リソースを作成する	
	instancingResource_ = directXBase_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);
	//座標変換行列リソースにデータを書き込むためのアドレスを取得してtransformationMatrixDataに割り当てる
	//書き込むためのアドレス
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	for (uint32_t i = 0; i < kNumMaxInstance; i++) {
		//単位行列を書き込んでおく
		instancingData_[i].WVP = Math::MakeIdentity4x4();
		instancingData_[i].World = Math::MakeIdentity4x4();
		instancingData_[i].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 初期色を白に設定
	}
}

//ワールドトランスフォームの更新
void ParticleSystem::UpdateWorldTransform(uint32_t numInstance, auto iterator) {
	//wvpの書き込み
	if (camera_) {
		//TransformからWorldMatrixを作る(ビルボード行列を入れた)
		worldMatrix_ = Rendering::MakeBillboardAffineMatrix(camera_->GetWorldMatrix(), (*iterator).transform);
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();

		instancingData_[numInstance].WVP = worldMatrix_ * viewProjectionMatrix;
	} else {
		instancingData_[numInstance].WVP = worldMatrix_;
	}
	//ワールド行列を送信
	instancingData_[numInstance].World = worldMatrix_;
}

//ストラクチャバッファの生成
void ParticleSystem::CreateStructuredBuffer() {
	//ストラクチャバッファを生成
	srvIndex_ = SRVManager::GetInstance()->Allocate();
	SRVManager::GetInstance()->CreateSRVForStructuredBuffer(
		srvIndex_,
		instancingResource_.Get(),
		kNumMaxInstance,
		sizeof(ParticleForGPU)
	);
}

//パーティクルの生成
Particle ParticleSystem::MakeNewParticle() {
	//位置と速度を[-1.0f,1.0f]でランダムに設定
	std::uniform_real_distribution<float>distribution(-1.0f, 1.0f);
	//パーティクルの初期化
	Particle particle;
	//拡縮
	particle.transform.scale = { 1.0f, 1.0f, 1.0f };
	//回転
	particle.transform.rotate = { 0.0f, Math::kPi, 0.0f };
	//位置
	Vector3 randomTranslate = { distribution(randomEngine_), distribution(randomEngine_), distribution(randomEngine_) };
	//パーティクルの位置を発生源を中心に設定
	particle.transform.translate = emitter_.transform.translate + randomTranslate;
	//速度
	particle.velocity = { distribution(randomEngine_), distribution(randomEngine_), distribution(randomEngine_) };

	//色の値を[0.0f,1.0f]でランダムに設定
	std::uniform_real_distribution<float>distColor(0.0f, 1.0f);
	//色
	particle.color = { distColor(randomEngine_), distColor(randomEngine_), distColor(randomEngine_),1.0f };

	//生存時間
	std::uniform_real_distribution<float>distTime(1.0f, 3.0f);
	particle.lifeTime = distTime(randomEngine_);
	particle.currentTime = 0.0f;

	return particle;
}

//パーティクルの発生
std::list<Particle> ParticleSystem::Emit() {
	std::list<Particle>particles;
	for (uint32_t i = 0; i < emitter_.count; i++) {
		particles.push_back(MakeNewParticle());
	}
	return particles;
}

//衝突判定
bool ParticleSystem::IsCollision(const AABB& aabb, const Vector3& point) {
	AABB temp = aabb;
	Vector3 tNear;
	Vector3 tFar;

	temp.min.x = (aabb.min.x - point.x);
	temp.max.x = (aabb.max.x - point.x);

	temp.min.y = (aabb.min.y - point.y);
	temp.max.y = (aabb.max.y - point.y);

	temp.min.z = (aabb.min.z - point.z);
	temp.max.z = (aabb.max.z - point.z);

	tNear.x = std::min(temp.min.x, temp.max.x);
	tFar.x = std::max(temp.min.x, temp.max.x);

	tNear.y = std::min(temp.min.y, temp.max.y);
	tFar.y = std::max(temp.min.y, temp.max.y);

	tNear.z = std::min(temp.min.z, temp.max.z);
	tFar.z = std::max(temp.min.z, temp.max.z);

	float tMin = std::max(tNear.x, std::max(tNear.z, tNear.y));
	float tMax = std::min(tFar.x, std::min(tFar.z, tFar.y));
	bool isCollision = false;
	if (tMin <= tMax) {
		if (tMin * tMax < 0.0f) {
			isCollision = true;
		}
		if (0.0f <= tMin && tMin <= 1.0f || 0.0f <= tMax && tMax <= 1.0f) {
			isCollision = true;
		}
	} else {
		isCollision = false;
	}
	return isCollision;
}
