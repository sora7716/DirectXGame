#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxgidebug.h>
#include "WinApp.h"
#include "2d/Log.h"
#include <dxcapi.h>
#include "math/Vector4.h"


/// <summary>
/// DirectXコモン
/// </summary>
class DirectXCommon {
public://メンバ関数
	DirectXCommon() = default;
	~DirectXCommon();
	/// <summary>
	/// DirectX12の初期化
	/// </summary>
	/// <param name="winApp">ウィンドウズアプリケーション</param>
	void InitializeDirectX12(WinApp*winApp);

	/// <summary>
	/// IDXIファクトリーの生成
	/// </summary>
	/// <returns>IDXIファクトリー</returns>
	IDXGIFactory7* MakeIDXGIFactory();

	/// <summary>
	/// 使用するアダプタを決定
	/// </summary>
	/// <returns>使用するアダプタ</returns>
	IDXGIAdapter4* DecideUseAdapter();

	/// <summary>
	/// D3D12デバイスの生成
	/// </summary>
	/// <returns>D3D12デバイス</returns>
	ID3D12Device* MakeD3D12Device();

	/// <summary>
	/// コマンドキューの生成
	/// </summary>
	/// <returns>コマンドキュー</returns>
	ID3D12CommandQueue* MakeCommandQueue();

	/// <summary>
	/// コマンドアローケータの生成 
	/// </summary>
	/// <returns>コマンドアローケータ</returns>
	ID3D12CommandAllocator* MakeCommandAllocator();

	/// <summary>
	/// コマンドリストの生成
	/// </summary>
	/// <returns>コマンドリスト</returns>
	ID3D12GraphicsCommandList* MakeCommandList();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	/// <param name="hwnd">メッセージが送信されたウィンドウのハンドル</param>
	/// <returns>スワップチェーン</returns>
	IDXGISwapChain4* MakeSwapChain();

	/// <summary>
	/// ディスクリプタヒープの生成
	/// </summary>
	/// <returns>ディスクリプタヒープ</returns>
	ID3D12DescriptorHeap* MakeDescriptorHeap();

	/// <summary>
	/// SwapChainからResourceを引っ張ってくる
	/// </summary>
	/// <param name="num">何番目か</param>
	/// <returns>リソース</returns>
	ID3D12Resource* BringResourcesFromSwapChain(UINT num);

	/// <summary>
	/// RTVの作成
	/// </summary>
	void MakeRTV();

	/// <summary>
	/// Fenceを作成する
	/// </summary>
	/// <returns></returns>
	ID3D12Fence* MakeFence();

	/// <summary>
	/// DXCの初期化
	/// </summary>
	void DXCInitialize();

	/// <summary>
	/// CompilerShader関数
	/// </summary>
	/// <param name="filePath">Shaderファイルパス</param>
	/// <param name="profile">Compileに使用するProfile</param>
	/// <returns></returns>
	IDxcBlob* CompileShader(const std::wstring& filePath,const wchar_t*profile);

	/// <summary>
	/// RootSignatureの生成
	/// </summary>
	/// <returns>RootSignature</returns>
	ID3D12RootSignature* MakeRootSignature();

	/// <summary>
	/// InputLayOutの設定
	/// </summary>
	void InputLayOutSetting();

	/// <summary>
	/// ブレンドステートの設定
	/// </summary>
	void BlendStateSetting();

	/// <summary>
	/// ラスタライザの設定
	/// </summary>
	void RasterizerSetting();

	/// <summary>
	/// シェーダをコンパイル
	/// </summary>
	void ShaderCompile();

	/// <summary>
	/// ゲームウィンドウの色を変更する
	/// </summary>
	void ChangeGameWindowColor();

	/// <summary>
	/// PSOの生成
	/// </summary>
	/// <returns>PSO</returns>
	ID3D12PipelineState* MakePSO();

	/// <summary>
	/// VertexResourceの生成
	/// </summary>
	/// <returns>VertexResource</returns>
	ID3D12Resource* MakeVertexResource();

	/// <summary>
	/// VertexBufferViewの生成
	/// </summary>
	void MakeVertexBufferView();

	/// <summary>
	/// 頂点リソースデータの書き込み
	/// </summary>
	/// <returns>頂点リソースデータ</returns>
	Vector4* VertexResourceData();

	/// <summary>
	/// ビューポートを生成
	/// </summary>
	void MakeViewport();

	/// <summary>
	/// シーザーを生成
	/// </summary>
	void MakeScissor();

	/// <summary>
	/// 三角形を描画
	/// </summary>
	void DrawTriangle();

	/// <summary>
	/// デバックレイヤー
	/// </summary>
	void DebugLayer();

	/// <summary>
	/// 実行を停止する(エラー・警告の場合)
	/// </summary>
	void StopExecution();

private://メンバ関数
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon operator=(const DirectXCommon&) = delete;
public://メンバ変数
	WinApp* winApp_ = nullptr;//ウィンドウズアプリケーション
	IDXGIFactory7* dxgiFactory_ = nullptr;//IDXIファクトリー
	IDXGIAdapter4*useAdapter_ = nullptr;//使用するアダプタ
	ID3D12Device* device_ = nullptr;//デバイス
	ID3D12CommandQueue* commandQueue_ = nullptr;//コマンドキュー
	ID3D12CommandAllocator*commandAllocator_ = nullptr;//コマンドアローケータ
	ID3D12GraphicsCommandList*commandList_ = nullptr;//コマンドリスト
	IDXGISwapChain4*swapChain_ = nullptr;//スワップチェーン
	ID3D12DescriptorHeap*rtvDescriptorHeap_ = nullptr;//ディスクリプタヒープ
	ID3D12Resource*swapChainResources_[2] = { nullptr };//スワップチェーンからリソースを引っ張ってくる
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];//RTVを2つ作るのでディスクリプタを2つ用意
	ID3D12Debug1* debugController_ = nullptr;//デバックコントローラー
	ID3D12Fence*fence_ = nullptr;//Fence
	uint64_t fenceValue_ = 0;//FenceValue
	HANDLE fenceEvent_ = 0;//FenceEvent
	//シェーダファイルの読み込みに使用するやつ↓
	IDxcUtils* dxcUtils_ = nullptr;
	IDxcCompiler3* dxcCompiler_ = nullptr;
	IDxcIncludeHandler* includeHandler_ = nullptr;
	//シェーダファイルの読み込みに使用するやつ↑
	ID3D12RootSignature* rootSignature_ = nullptr;//RootSignature
	//InputLayOut
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[1] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_ = {};
	D3D12_BLEND_DESC blendDesc_{};//BlendState
	D3D12_RASTERIZER_DESC rasterizerDesc_{};//Rasterizer
	IDxcBlob* vertexShaderBlob = nullptr;//vertexShaderBlob
	IDxcBlob* pixelShaderBlob = nullptr;//pixelShaderBlob
	ID3D12PipelineState* graphicsPiplineState_ = nullptr;//PSO
	ID3D12Resource* vertexResource_ = nullptr;//VertexResource
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};//VertexBufferView
	Vector4* vertexData_ = nullptr;//頂点リソースデータ
	D3D12_VIEWPORT viewport_{};//ビューポート
	D3D12_RECT scissorRect_{};//シーザー矩形
};

