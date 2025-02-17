#pragma once
#include "engine/math/ResourceData.h"
#include "engine/math/rendering/Rendering.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <wrl.h>

//前方宣言
class SpriteGeneral;
class DirectXBase;

/// <summary>
/// スプライト
/// </summary>
class Sprite {
public://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(SpriteGeneral* spriteGeneral);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const D3D12_GPU_DESCRIPTOR_HANDLE& texture);
private://メンバ関数
	/// <summary>
	/// 頂点データの初期化
	/// </summary>
	void InitialilzeVertexData();

	/// <summary>
	/// インデックスの初期化
	/// </summary>
	void InitializeIndexData();

	/// <summary>
	/// マテリアルデータの初期化
	/// </summary>
	void InitialilzeMaterialData();

	/// <summary>
	/// 座標変換行列の初期化
	/// </summary>
	void InitializeTransformationMatrixData();
private://メンバ変数
	Transform transform_ = {};//トランスフォームの情報
	SpriteGeneral* spriteGeneral_ = nullptr;//スプライトの共通部分
	DirectXBase* directXBase_ = nullptr;//DirectXの基盤
	//バッファリソース
	ComPtr<ID3D12Resource>vertexResource_ = nullptr;//頂点
	ComPtr<ID3D12Resource>indexResource_ = nullptr;//インデックス
	ComPtr<ID3D12Resource>materialResource_ = nullptr;//マテリアル
	ComPtr<ID3D12Resource>transformationMatrixResoruce_ = nullptr;//座標変換行列1
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;//頂点
	uint32_t* indexData_ = nullptr;//インデックス
	Material* materialData_ = nullptr;//マテリアル
	TransformationMatrix* transformationMatrixData_ = nullptr;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};//頂点
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};//インデックス
};

