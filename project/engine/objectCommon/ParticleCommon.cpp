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
void ParticleCommon::Initialize(DirectXBase* directXBase) {
	assert(directXBase);//Nullチェック
	directXBase_ = directXBase;//DirectXの基盤を受け取る
	//ブレンド
	blend_ = std::make_unique<Blend>();
	//グラフィックスパイプラインの生成と初期化
	makeGraphicsPipeline_ = std::make_unique<GraphicsPipeline>();
	//DirectXの基盤部分をセットする
	makeGraphicsPipeline_->SetDirectXBase(directXBase_);
	makeGraphicsPipeline_->SetVertexShaderFileName(L"Particle.VS.hlsl");
	makeGraphicsPipeline_->SetPixelShaderFileName(L"Particle.PS.hlsl");
	//シグネイチャBlobの初期化
	makeGraphicsPipeline_->CreateRootSignatureBlobForSRV();
	//ルートシグネイチャの保存
	makeGraphicsPipeline_->CreateRootSignature();
	//インプットレイアウト
	makeGraphicsPipeline_->InitializeInputLayoutDesc();
	//ラスタライザステート
	makeGraphicsPipeline_->InitializeRasterizerSatate();
	//頂点シェーダBlob
	makeGraphicsPipeline_->CompileVertexShader();
	//ピクセルシェーダBlob
	makeGraphicsPipeline_->CompilePixelShader();
	//PSO
	for (uint32_t i = 0; i < static_cast<int32_t>(BlendMode::kCountOfBlendMode); i++) {
		//ブレンドステート
		makeGraphicsPipeline_->InitializeBlendState(i);
		//グラフィックスパイプラインの生成[
		graphicsPipelineStates_[i] = makeGraphicsPipeline_->CreateGraphicsPipeline();
	}	//ルートシグネイチャの記録
	rootSignature_ = makeGraphicsPipeline_->GetRootSignature();
	//グラフィックスパイプラインステートの記録
	//graphicsPipelineStates_ = makeGraphicsPipeline_->GetGraphicsPipelines();
}

//終了
void ParticleCommon::Finalize() {
	delete instance;
	instance = nullptr;
	isFinalize = true;
}