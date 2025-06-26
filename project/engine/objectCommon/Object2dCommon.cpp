
#include "Object2dCommon.h"
#include "engine/base/DirectXBase.h"
#include "engine/camera/Camera.h"
#include "engine/base/GraphicsPipeline.h"
#include <cassert>
using namespace Microsoft::WRL;

//インスタンスのゲッター
Object2dCommon* Object2dCommon::GetInstance() {
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new Object2dCommon();
	}
	return instance;
}

//初期化
void Object2dCommon::Initialize(DirectXBase* directXBase) {
	assert(directXBase);//Nullチェック
	directXBase_ = directXBase;//DirectXの基盤を受け取る
	//ブレンド
	blend_ = new Blend();
	//グラフィックスパイプラインの生成と初期化
	makeGraphicsPipeline_ = new GraphicsPipeline();
	//シェーダを設定
	makeGraphicsPipeline_->SetVertexShaderFileName(L"Object3d.VS.hlsl");
	makeGraphicsPipeline_->SetPixelShaderFileName(L"Object3d.PS.hlsl");
	makeGraphicsPipeline_->Initialize(directXBase_);
	//ルートシグネイチャの記録
	rootSignature_ = makeGraphicsPipeline_->GetRootSignature();
	//グラフィックスパイプラインステートの記録
	graphicsPipelineStates_ = makeGraphicsPipeline_->GetGraphicsPipelines();

}

//共通描画設定
void Object2dCommon::DrawSetting() {
	//ルートシグネイチャをセットするコマンド
	directXBase_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
	//プリミティブトポロジーをセットするコマンド
	directXBase_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//光源の生成
void Object2dCommon::CreateDirectionLight() {
	//光源のリソースを作成
	directionalLightResource_ = directXBase_->CreateBufferResource(sizeof(DirectionalLight));
	//光源データの書きこみ
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;
}

//DirectionalLightのセッター
void Object2dCommon::SetDirectionalLightData(const DirectionalLight& directionalLightData) {
	directionalLightData_->color = directionalLightData.color;
	directionalLightData_->direction = directionalLightData.direction;
	directionalLightData_->intensity = directionalLightData.intensity;
}

//DirectionalLightのリソースのゲッター
ID3D12Resource* Object2dCommon::GetDirectionalLightResource()const {
	return directionalLightResource_.Get();
}

//DirectXの基盤のゲッター
DirectXBase* Object2dCommon::GetDirectXBase() const {
	return directXBase_;
}

//グラフィックパイプラインのゲッター
std::array<ComPtr<ID3D12PipelineState>, static_cast<int32_t>(BlendMode::kCountOfBlendMode)> Object2dCommon::GetGraphicsPipelineStates() const {
	return graphicsPipelineStates_;
}
//終了
void Object2dCommon::Finalize() {
	delete blend_;
	delete makeGraphicsPipeline_;
	delete instance;
	instance = nullptr;
	isFinalize = true;
}

// デフォルトカメラのセッター
void Object2dCommon::SetDefaultCamera(Camera* camera) {
	defaultCamera_ = camera;
}

// デフォルトカメラのゲッター
Camera* Object2dCommon::GetDefaultCamera() const {
	return defaultCamera_;
}
