#include "BaseObjectCommon.h"
#include "engine/base/DirectXBase.h"
#include <cassert>
using namespace Microsoft::WRL;

//初期化
void BaseObjectCommon::Initialize(DirectXBase* directXBase) {
	assert(directXBase);//Nullチェック
	directXBase_ = directXBase;//DirectXの基盤を受け取る
	//ブレンド
	blend_ = std::make_unique<Blend>();
	//グラフィックスパイプラインの生成と初期化
	makeGraphicsPipeline_ = std::make_unique<GraphicsPipeline>();
	makeGraphicsPipeline_->Initialize(directXBase_);
	//シェーダを設定
	makeGraphicsPipeline_->SetVertexShaderFileName(L"Object3d.VS.hlsl");
	makeGraphicsPipeline_->SetPixelShaderFileName(L"Object3d.PS.hlsl");
	//ルートシグネイチャの記録
	rootSignature_ = makeGraphicsPipeline_->GetRootSignature();
	//グラフィックスパイプラインステートの記録
	graphicsPipelineStates_ = makeGraphicsPipeline_->GetGraphicsPipelines();
}

//共通描画設定
void BaseObjectCommon::DrawSetting() {
	//ルートシグネイチャをセットするコマンド
	directXBase_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
	//プリミティブトポロジーをセットするコマンド
	directXBase_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//光源の生成
void BaseObjectCommon::CreateDirectionLight() {
	//光源のリソースを作成
	directionalLightResource_ = directXBase_->CreateBufferResource(sizeof(DirectionalLight));
	//光源データの書きこみ
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;
}

//DirectionalLightのセッター
void BaseObjectCommon::SetDirectionalLightData(const DirectionalLight& directionalLightData) {
	directionalLightData_->color = directionalLightData.color;
	directionalLightData_->direction = directionalLightData.direction;
	directionalLightData_->intensity = directionalLightData.intensity;
}

//DirectionalLightのリソースのゲッター
ID3D12Resource* BaseObjectCommon::GetDirectionalLightResource()const {
	return directionalLightResource_.Get();
}

//DirectXの基盤のゲッター
DirectXBase* BaseObjectCommon::GetDirectXBase() const {
	return directXBase_;
}

//グラフィックパイプラインのゲッター
std::array<ComPtr<ID3D12PipelineState>, static_cast<int32_t>(BlendMode::kCountOfBlendMode)> BaseObjectCommon::GetGraphicsPipelineStates() const {
	return graphicsPipelineStates_;
}

// デフォルトカメラのセッター
void BaseObjectCommon::SetDefaultCamera(Camera* camera) {
	defaultCamera_ = camera;
}

// デフォルトカメラのゲッター
Camera* BaseObjectCommon::GetDefaultCamera() const {
	return defaultCamera_;
}