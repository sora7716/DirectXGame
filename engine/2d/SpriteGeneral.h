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
	/// <param name="directXBase">DirectXの基盤</param>
	/// <param name="graphicsPiplineDesc">グラフィックスパイプラインデスク</param>
	void Initialize(DirectXBase* directXBase, D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPiplineDesc);

	/// <summary>
	/// 共通描画設定
	/// </summary>
	void DrawSetting();

	/// <summary>
	/// DirectXの基盤のゲッター
	/// </summary>
	/// <returns>DirectXの基盤</returns>
	DirectXBase* GetDirectXBase()const;
private://メンバ関数
	/// <summary>
	/// ルートシグネイチャの設定
	/// </summary>
	void InitializeRootSigneture();

	/// <summary>
	/// ルートシグネイチャの生成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
    /// グラフィックスパイプラインの生成
    /// </summary>
	/// <param name="graphicsPiplineDesc">グラフィックスパイプラインデスク</param>
	void CreateGraphicsPipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPiplineDesc);
private://メンバ変数
	DirectXBase* directXBase_ = nullptr;//DirectXの基盤
	//Blob
	ComPtr<ID3DBlob> signatureBlob_ = nullptr;
	ComPtr<ID3DBlob> errorBlob_ = nullptr;
	//ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	//グラフィックスパイプライン(PSO)
	ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;
};

