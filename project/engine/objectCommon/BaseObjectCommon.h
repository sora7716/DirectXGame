#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "engine/math/ResourceData.h"

//前方宣言
class DirectXBase;
/// <summary>
/// オブジェクトの管理する基底クラス
/// </summary>
class BaseObjectCommon {
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
	/// <param name="directionalLightData">directionalLightのデータ</param>
	virtual void Initialize(DirectXBase* directXBase);

	/// <summary>
	/// 共通描画設定
	/// </summary>
	virtual void DrawSetting();

	/// <summary>
    /// 光源の生成
    /// </summary>
	void CreateDirectionLight();

	/// <summary>
	/// DirectionalLightのセッター
	/// </summary>
	/// <param name="directionalLightData">DirectionalLightデータ</param>
	void SetDirectionalLightData(const DirectionalLight&directionalLightData);

	/// <summary>
	/// DirectionalLightのリソースのゲッター
	/// </summary>
	/// <returns>DirectionalLightのリソース</returns>
	ID3D12Resource* GetDirectionalLightResource();

	/// <summary>
	/// DirectXの基盤のゲッター.
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
	void CreateGraphicsPipeline();
protected://メンバ変数
	DirectXBase* directXBase_ = nullptr;//DirectXの基盤
	//Blob
	ComPtr<ID3DBlob> signatureBlob_ = nullptr;
	ComPtr<ID3DBlob> errorBlob_ = nullptr;
	//ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	//グラフィックスパイプライン(PSO)
	ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;
	//バッファリソース
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;//光源
	//バッファリソース内のデータを指すポインタ
	DirectionalLight* directionalLightData_ = nullptr;//光源
};

