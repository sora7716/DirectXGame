#pragma once
#include "engine/blend/Blend.h"
#include <dxcapi.h>
#include <wrl.h>
#include <d3d12.h>
#include <cstdint>
#include <memory>
#include <array>
#include <string>
//前方宣言
class DirectXBase;

/// <summary>
/// グラフィックスパイプライン
/// </summary>
class GraphicsPipeline {
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GraphicsPipeline() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GraphicsPipeline() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXBase">DirectXの基盤部分</param>
	void Initialize(DirectXBase* directXBase);

	/// <summary>
	/// ルートシグネイチャのゲッター
	/// </summary>
	/// <returns>ルートシグネイチャ</returns>
	ComPtr<ID3D12RootSignature>GetRootSignature();

	/// <summary>
	/// グラフィックスパイプライン達のゲッター
	/// </summary>
	/// <returns>グラフィックスパイプライン達</returns>
	std::array<ComPtr<ID3D12PipelineState>, static_cast<int32_t>(BlendMode::kCountOfBlendMode)>GetGraphicsPipelines();

	/// <summary>
	/// 頂点シェーダのファイル名をセット
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void SetVertexShaderFileName(const std::wstring& fileName);

	/// <summary>
	/// ピクセルシェーダのファイル名をセット
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void SetPixelShaderFileName(const std::wstring& fileName);

	/// <summary>
	/// ルートシグネイチャBlobの生成(CBV)
	/// </summary>
	void CreateRootSignatureBlobForCBV();

	/// <summary>
	/// ルートシグネイチャBlobの生成(SRV)
	/// </summary>
	void CreateRootSignatureBlobForSRV();

	/// <summary>
	/// ルートシグネイチャの生成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// インプットレイアウトの初期化
	/// </summary>
	void InitializeInputLayoutDesc();

	/// <summary>
	/// ブレンドステートの初期化
	/// </summary>
	/// <param name="blendMode">ブレンドモード</param>
	void InitializeBlendState(int32_t blendMode);

	/// <summary>
	/// ラスタライザステートの初期化
	/// </summary>
	void InitializeRasterizerSatate();

	/// <summary>
	/// 頂点シェーダのコンパイル
	/// </summary>
	void CompileVertexShader();

	/// <summary>
	/// ピクセルシェーダのコンパイル
	/// </summary>
	void CompilePixelShader();

	/// <summary>
	/// PSOの生成
	/// </summary>
	ComPtr<ID3D12PipelineState> CreateGraphicsPipeline();

	/// <summary>
	/// DirectXの基盤のセッター
	/// </summary>
	/// <param name="directXBase">DirectXの基盤部分</param>
	void SetDirectXBase(DirectXBase* directXBase);
protected://メンバ変数
	//DirectXの基盤
	DirectXBase* directXBase_ = nullptr;
	//ルートシグネイチャ
	ComPtr<ID3D12RootSignature>rootSignature_ = nullptr;
	//インプットレイアウト
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_ = {};
	//ブレンドステート
	D3D12_BLEND_DESC blendDesc_ = {};
	//ラスタライザステート
	D3D12_RASTERIZER_DESC rasterizerDesc_ = {};
	//頂点シェーダBlob
	ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
	//ピクセルシェーダBlob
	ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;
	//グラフィックスパイプライン達
	std::array<ComPtr<ID3D12PipelineState>, static_cast<int32_t>(6)>graphicsPipelines_ = { nullptr };
	//ファイル名
	std::wstring vertexShaderFileName_ = L"Object3d.VS.hlsl";//頂点
	std::wstring pixelShaderFileName_ = L"Object3d.PS.hlsl";//ピクセル
	//ルートシグネイチャBlob
	ComPtr<ID3DBlob>signatureBlob_ = nullptr;
};
