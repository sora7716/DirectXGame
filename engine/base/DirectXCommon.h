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

private://メンバ関数
	/// <summary>
	/// IDXIファクトリーの生成
	/// </summary>
	void MakeIDXGIFactory();

	/// <summary>
	/// 使用するアダプタを決定
	/// </summary>
	void DecideUseAdapter();

	/// <summary>
	/// D3D12デバイスの生成
	/// </summary>
	void MakeD3D12Device();

	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon operator=(const DirectXCommon&) = delete;
private://メンバ変数
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;//IDXIファクトリー
	Microsoft::WRL::ComPtr<IDXGIAdapter4>useAdapter_ = nullptr;//使用するアダプタ
	HRESULT hr_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;//デバイス
};

