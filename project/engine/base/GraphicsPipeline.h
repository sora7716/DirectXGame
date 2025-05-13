
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
private://メンバ関数
	/// <summary>
	/// ルートシグネイチャBlobの生成(CBV)
	/// </summary>
	/// <returns>ルートシグネイチャのBlob</returns>
	ComPtr<ID3DBlob>CreateRootSignatureBlobForCBV();

	/// <summary>
    /// ルートシグネイチャBlobの生成(SBV)
    /// </summary>
    /// <returns>ルートシグネイチャのBlob</returns>
	ComPtr<ID3DBlob>CreateRootSignatureBlobForSBV();

	/// <summary>
	/// ルートシグネイチャの生成
	/// </summary>
	/// <returns>ルートシグネイチャ</returns>
	ComPtr<ID3D12RootSignature>CreateRootSignature();

	/// <summary>
	/// インプットレイアウトの初期化
	/// </summary>
	/// <returns>インプットレイアウトデスク</returns>
	D3D12_INPUT_LAYOUT_DESC InitializeInputLayoutDesc();

	/// <summary>
	/// ブレンドステートの初期化
	/// </summary>
	/// <param name="blendMode">ブレンドモード</param>
	/// <returns>ブレンドデスク</returns>
	D3D12_BLEND_DESC InitializeBlendState(int32_t blendMode);

	/// <summary>
	/// ラスタライザステートの初期化
	/// </summary>
	/// <returns>ラスタライザデスク</returns>
	D3D12_RASTERIZER_DESC InitializeRasterizerSatate();

	/// <summary>
	/// 頂点シェーダのコンパイル
	/// </summary>
	/// <returns>頂点シェーダBlob</returns>
	ComPtr<IDxcBlob>CompileVertexShader();

	/// <summary>
	/// ピクセルシェーダのコンパイル
	/// </summary>
	/// <returns>ピクセルシェーダBlob</returns>
	ComPtr<IDxcBlob>CompilePixelShader();

	/// <summary>
	/// PSOの生成
	/// </summary>
	/// <returns>グラフィックスパイプライン</returns>
	ComPtr<ID3D12PipelineState> CreateGraphicsPipeline();
private://メンバ変数
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
