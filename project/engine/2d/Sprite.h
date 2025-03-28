#pragma once
#include "engine/math/ResourceData.h"
#include "engine/math/func/Rendering.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <wrl.h>
#include <string>

//前方宣言
class DirectXBase;

/// <summary>
/// スプライト
/// </summary>
class Sprite {
private://エイリアステンプレート
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
	/// <param name="textureFilePath">テクスチャのファイルパス</param>
	void Initialize(std::string textureFilePath);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// テクスチャの変更
	/// </summary>
	/// <param name="textureFilePath">テクスチャファイルパス</param>
	void ChangeTexture(std::string textureFilePath);

	/// <summary>
	/// UVの座標変換の更新
	/// </summary>
	/// <param name="uvTransform">uv座標</param>
	void UpdateUVTransform(Transform2D uvTransform);
	
	/// <summary>
	/// 色のゲッター
	/// </summary>
	/// <returns>色</returns>
	const Vector4& GetColor()const;

	/// <summary>
	/// 色のセッター
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color);
private://メンバ関数
	/// <summary>
	/// 頂点データの初期化
	/// </summary>
	void InitializeVertexData();

	/// <summary>
	/// 頂点リソースの生成
	/// </summary>
	void CreateVertexResorce();

	/// <summary>
	/// インデックスデータの初期化
	/// </summary>
	void InitializeIndexData();

	/// <summary>
	/// インデックスリソースの生成
	/// </summary>
	void CreateIndexResorce();

	/// <summary>
	/// マテリアルデータの初期化
	/// </summary>
	void InitializeMaterialData();

	/// <summary>
	/// マテリアルリソースの生成
	/// </summary>
	void CreateMaterialResorce();
private://メンバ変数
	//テクスチャ番号
	std::string filePath_ = {};
	Transform transform_ = {};//トランスフォームの情報
	DirectXBase* directXBase_ = nullptr;//DirectXの基盤
	//バッファリソース
	ComPtr<ID3D12Resource>vertexResource_ = nullptr;//頂点
	ComPtr<ID3D12Resource>indexResource_ = nullptr;//インデックス
	ComPtr<ID3D12Resource>materialResource_ = nullptr;//マテリアル
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;//頂点
	uint32_t* indexData_ = nullptr;//インデックス
	Material* materialData_ = nullptr;//マテリアル
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};//頂点
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};//インデックス
};

