#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <dxgidebug.h>
#include "WinApp.h"
#include "2d/Log.h"

/// <summary>
/// DirectXコモン
/// </summary>
class DirectXCommon final {
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns>インスタンス</returns>
	static DirectXCommon* GetInstance();

	/// <summary>
	/// DirectX12の初期化
	/// </summary>
	/// <param name="hwnd">メッセージが送信されたウィンドウのハンドル</param>
	void InitializeDirectX12();

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
	/// ゲームウィンドウの色を変更する
	/// </summary>
	void ChangeGameWindowColor();

	/// <summary>
	/// デバックレイヤー
	/// </summary>
	void DebugLayer();

	/// <summary>
	/// 実行を停止する(エラー・警告の場合)
	/// </summary>
	void StopExecution();

	/// <summary>
	/// リソースリークチェック
	/// </summary>
	void ResourceLeakCheck();

private://メンバ関数
	DirectXCommon() = default;
	~DirectXCommon();
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon operator=(const DirectXCommon&) = delete;
private://メンバ変数
	HRESULT hr_;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;//IDXIファクトリー
	Microsoft::WRL::ComPtr<IDXGIAdapter4>useAdapter_ = nullptr;//使用するアダプタ
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;//デバイス
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;//コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>commandAllocator_ = nullptr;//コマンドアローケータ
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>commandList_ = nullptr;//コマンドリスト
	Microsoft::WRL::ComPtr<IDXGISwapChain4>swapChain_ = nullptr;//スワップチェーン
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>rtvDescriptorHeap_ = nullptr;//ディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12Resource>swapChainResources_[2] = { nullptr };//スワップチェーンからリソースを引っ張ってくる
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];//RTVを2つ作るのでディスクリプタを2つ用意
	Microsoft::WRL::ComPtr<ID3D12Debug1> debugController_ = nullptr;//デバックコントローラー
	Microsoft::WRL::ComPtr<ID3D12Fence>fence_ = nullptr;//Fence
	uint64_t fenceValue_ = 0;//FenceValue
	HANDLE fenceEvent_ = 0;//FenceEvent
};

