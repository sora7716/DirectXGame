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
class SpriteCommon;
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
	/// <param name="spriteCommon">スプライトの共通部分</param>
	/// <param name="textureFilePath">テクスチャのファイルパス</param>
	void Initialize(SpriteCommon* spriteCommon,std::string textureFilePath);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

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
	/// サイズのゲッター
	/// </summary>
	/// <returns>サイズ</returns>
	const Vector2& GetScale()const;

	/// <summary>
	/// 回転のゲッター
	/// </summary>
	/// <returns>回転</returns>
	float GetRotate()const;

	/// <summary>
	/// 位置のゲッター
	/// </summary>
	/// <returns>位置</returns>
	const Vector2& GetTranslate()const;

	/// <summary>
	/// UVのサイズのゲッター
	/// </summary>
	/// <returns>UVサイズ</returns>
	const Vector2& GetUVScale()const;

	/// <summary>
	/// UVの回転のゲッター
	/// </summary>
	/// <returns>UVの回転</returns>
	float GetUVRotate()const;

	/// <summary>
	/// UVの位置のゲッター
	/// </summary>
	/// <returns>UVの位置</returns>
	const Vector2& GetUVTranslate()const;

	/// <summary>
	/// 色のゲッター
	/// </summary>
	/// <returns>色</returns>
	const Vector4& GetColor()const;

	/// <summary>
	/// サイズのセッター
	/// </summary>
	/// <param name="scale">サイズ</param>
	void SetScale(const Vector2& scale);

	/// <summary>
	/// 回転のセッター
	/// </summary>
	/// <param name="rotate">回転</param>
	void SetRotate(float rotate);

	/// <summary>
	/// 位置のセッター
	/// </summary>
	/// <param name="translate">位置</param>
	void SetTranslate(const Vector2& translate);

	/// <summary>
	/// UVのサイズのセッター
	/// </summary>
	/// <param name="scale">サイズ</param>
	void SetUVScale(const Vector2& scale);

	/// <summary>
	/// UVの回転のセッター
	/// </summary>
	/// <param name="rotate">回転</param>
	void SetUVRotate(float rotate);

	/// <summary>
	/// UVの位置のゲッター
	/// </summary>
	/// <param name="translate">位置</param>
	void SetUVTranslate(const Vector2& translate);

	/// <summary>
	/// 色のセッター
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color);
private://メンバ関数
	/// <summary>
	/// 頂点データの初期化
	/// </summary>
	void InitialilzeVertexData();

	/// <summary>
	/// 頂点データの生成
	/// </summary>
	void CreateVertexData();

	/// <summary>
	/// インデックスデータの初期化
	/// </summary>
	void InitializeIndexData();

	/// <summary>
	/// インデックスデータの生成
	/// </summary>
	void CreateIndexData();

	/// <summary>
	/// マテリアルデータの初期化
	/// </summary>
	void InitializeMaterialData();

	/// <summary>
	/// マテリアルデータの生成
	/// </summary>
	void CreateMaterialData();

	/// <summary>
	/// 座標変換行列データの初期化
	/// </summary>
	void InitializeTransformationMatrixData();

	/// <summary>
	/// 座標変換行列データの生成
	/// </summary>
	void CreateTransformationMatrixData();

	/// <summary>
	/// 光源の生成
	/// </summary>
	void CreateDirectionLight();

	/// <summary>
	/// 座標変換の更新
	/// </summary>
	void UpdateTransform();

	/// <summary>
	/// UVの座標変換の更新
	/// </summary>
	void UpdateUVTransform();
private://メンバ変数
	//テクスチャ番号
	uint32_t textureIndex = 0;
	//ローカル座標
	Transform2D transform2D_ = {
		.scale = { 640.0f,360.0f },
		.rotate = 0.0f,
		.translate = {0.0f,0.0f}
	};
	//UV座標
	Transform2D uvTransform_ = {
		.scale = { 1.0f,1.0f },
		.rotate = 0.0f,
		.translate = {0.0f,0.0f}
	};
	Transform transform_ = {};//トランスフォームの情報
	SpriteCommon* spriteCommon_ = nullptr;//スプライトの共通部分
	DirectXBase* directXBase_ = nullptr;//DirectXの基盤
	//バッファリソース
	ComPtr<ID3D12Resource>vertexResource_ = nullptr;//頂点
	ComPtr<ID3D12Resource>indexResource_ = nullptr;//インデックス
	ComPtr<ID3D12Resource>materialResource_ = nullptr;//マテリアル
	ComPtr<ID3D12Resource>transformationMatrixResoruce_ = nullptr;//座標変換行列
	ComPtr<ID3D12Resource>directionalLightResource_ = nullptr;//光源
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;//頂点
	uint32_t* indexData_ = nullptr;//インデックス
	Material* materialData_ = nullptr;//マテリアル
	DirectionalLight* directionalLightData_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};//頂点
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};//インデックス
};

