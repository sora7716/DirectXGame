#include "ParticleCommon.h"
using namespace Microsoft::WRL;

//インスタンスのゲッター
ParticleCommon* ParticleCommon::GetInstance() {
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new ParticleCommon();
	}
	return instance;
}

//初期化
void ParticleCommon::Initialize(DirectXBase* directXBase){
	assert(directXBase);//Nullチェック
	directXBase_ = directXBase;//DirectXの基盤を受け取る
	//ブレンド
	blend_ = std::make_unique<Blend>();
	//グラフィックスパイプラインの生成と初期化
	graphicsPipeline_ = std::make_unique<GraphicsPipeline>();
	graphicsPipeline_->SetVertexShaderFileName(L"ParticleVS.hlsl");
	graphicsPipeline_->SetPixelShaderFileName(L"ParticlePS.hlsl");
	graphicsPipeline_->Initialize(directXBase_);
	//ルートシグネイチャの記録
	rootSignature_ = graphicsPipeline_->GetRootSignature();
	//グラフィックスパイプラインステートの記録
	graphicsPipelineStates_ = graphicsPipeline_->GetGraphicsPipelines();
}

//終了
void ParticleCommon::Finalize() {
	delete instance;
	instance = nullptr;
	isFinalize = true;
}