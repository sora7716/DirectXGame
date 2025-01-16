#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
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
	void InitializeDirectX12();

	/// <summary>
	/// IDXIファクトリーの生成
	/// </summary>
	IDXGIFactory7* MakeIDXGIFactory();

	/// <summary>
	/// 使用するアダプタを決定
	/// </summary>
	IDXGIAdapter4* DecideUseAdapter();

	/// <summary>
	/// D3D12デバイスの生成
	/// </summary>
	ID3D12Device* MakeD3D12Device();

	/// <summary>
	/// コマンドキューの生成
	/// </summary>
	ID3D12CommandQueue* MakeCommandQueue();

	/// <summary>
	/// コマンドアローケータの生成
	/// </summary>
	ID3D12CommandAllocator* MakeCommandAllocator();

	/// <summary>
	/// コマンドリストの生成
	/// </summary>
	ID3D12GraphicsCommandList* MakeCommandList();

private://メンバ関数
	DirectXCommon() = default;
	~DirectXCommon() = default;
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
};

