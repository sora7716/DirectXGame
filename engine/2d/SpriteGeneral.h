#pragma once
#include "engine/base/DirectXBase.h"

/// <summary>
/// スプライトの共通部分
/// </summary>
class SpriteGeneral {
public://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SpriteGeneral() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SpriteGeneral() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="cullMode">カリングモード</param>
	/// <param name="fillMode">塗りつぶすか</param>
	void Initialize(D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK, D3D12_FILL_MODE fillMode = D3D12_FILL_MODE_SOLID);
private://メンバ関数
	/// <summary>
	/// ルートシグネイチャの設定
	/// </summary>
	void InitializeRootSigneture();

	/// <summary>
	/// デスクリプターレンジの初期化
	/// </summary>
	void InitializeDescriptorRange();

	/// <summary>
	/// ルートパラメータの初期化
	/// </summary>
	void InitializeRootParameter();

	/// <summary>
	/// SignatureBlobの生成
	/// </summary>
	void CreateSignatureBlob();

	/// <summary>
	/// ルートシグネイチャの生成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// インプットレイアウトの初期化
	/// </summary>
	void InitializeInputLayout();

	/// <summary>
	/// ブレンドステートの初期化
	/// </summary>
	void InitializeBlendState();

	/// <summary>
	/// ラスタライザステートの初期化
	/// </summary>
	/// <param name="cullMode">カリングモード</param>
	/// <param name="fillMode">塗りつぶすかどうか</param>
	void InitializeRasterizerState(D3D12_CULL_MODE cullMode, D3D12_FILL_MODE fillMode);

	/// <summary>
	/// シェーダをコンパイル
	/// </summary>
	void ShaderCompile();

	/// <summary>
	/// グラフィックスパイプラインの生成
	/// </summary>
	void CreateGraphicsPipeline();
private://メンバ変数
	DirectXBase* directXBase_ = nullptr;//DirectXの基盤
	//ルートシグネイチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignetureDesc_{};
	//デスクリプターレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange_[1] = {};
	//ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters_[4] = {};
	//Blob
	ComPtr<ID3DBlob> signatureBlob_ = nullptr;
	ComPtr<ID3DBlob> errorBlob_ = nullptr;
	//ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	//インプットレイアウト
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	//ブレンドステート
	D3D12_BLEND_DESC blendDesc_{};
	//ラスタライザステート
	D3D12_RASTERIZER_DESC rasterizerDesc_{};
	//頂点シェーダ
	ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
	//ピクセルシェーダ
	ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;
	//グラフィックスパイプライン(PSO)
	ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;
};

