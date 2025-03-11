#pragma once
#include <wrl.h>
#include <d3d12.h>
//前方宣言
class DirectXBase;
/// <summary>
/// オブジェクトの管理する基底クラス
/// </summary>
class BaseObjectCommon{
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BaseObjectCommon() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual~BaseObjectCommon() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXBase">DirectXの基盤</param>
	/// <param name="graphicsPiplineDesc">グラフィックスパイプラインデスク</param>
	virtual void Initialize(DirectXBase* directXBase, D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPiplineDesc);

	/// <summary>
	/// 共通描画設定
	/// </summary>
	virtual void DrawSetting();

	/// <summary>
	/// DirectXの基盤のゲッター
	/// </summary>
	/// <returns>DirectXの基盤</returns>
	DirectXBase* GetDirectXBase()const;
protected://メンバ関数
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
protected://メンバ変数
	DirectXBase* directXBase_ = nullptr;//DirectXの基盤
	//Blob
	ComPtr<ID3DBlob> signatureBlob_ = nullptr;
	ComPtr<ID3DBlob> errorBlob_ = nullptr;
	//ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	//グラフィックスパイプライン(PSO)
	ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;
};

