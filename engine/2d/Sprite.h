#pragma once
#include "engine/math/ResourceData.h"
#include "engine/math/rendering/Rendering.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <wrl.h>
#include <string>

//前方宣言
class SpriteGeneral;
class DirectXBase;

// 2次元用の座標変換
typedef struct Transform2D {
	Vector2 size;
	float rotation;
	Vector2 translation;
}Transform2D;

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
	/// <param name="spriteGeneral">スプライトの共通部分</param>
	/// <param name="textureFilePath">テクスチャのファイルパス</param>
	void Initialize(SpriteGeneral* spriteGeneral,std::string textureFilePath);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// サイズのゲッター
	/// </summary>
	/// <returns>サイズ</returns>
	const Vector2& GetSize()const;

	/// <summary>
	/// 回転のゲッター
	/// </summary>
	/// <returns>回転</returns>
	float GetRotation()const;

	/// <summary>
	/// 位置のゲッター
	/// </summary>
	/// <returns>位置</returns>
	const Vector2& GetPosition()const;

	/// <summary>
	/// UVのサイズのゲッター
	/// </summary>
	/// <returns>UVサイズ</returns>
	const Vector2& GetUVSize()const;

	/// <summary>
	/// UVの回転のゲッター
	/// </summary>
	/// <returns>UVの回転</returns>
	float GetUVRotation()const;

	/// <summary>
	/// UVの位置のゲッター
	/// </summary>
	/// <returns>UVの位置</returns>
	const Vector2& GetUVPosition()const;

	/// <summary>
	/// 色のゲッター
	/// </summary>
	/// <returns>色</returns>
	const Vector4& GetColor()const;

	/// <summary>
	/// サイズのセッター
	/// </summary>
	/// <param name="size">サイズ</param>
	void SetSize(const Vector2& size);

	/// <summary>
	/// 回転のセッター
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(float rotation);

	/// <summary>
	/// 位置のセッター
	/// </summary>
	/// <param name="position">位置</param>
	void SetPosition(const Vector2& position);

	/// <summary>
	/// UVのサイズのセッター
	/// </summary>
	/// <param name="size">サイズ</param>
	void SetUVSize(const Vector2& size);

	/// <summary>
	/// UVの回転のセッター
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetUVRoation(float rotation);

	/// <summary>
	/// UVの位置のゲッター
	/// </summary>
	/// <param name="position">位置</param>
	void SetUVPosition(const Vector2& position);

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
		.size = { 640.0f,360.0f },
		.rotation = 0.0f,
		.translation = {0.0f,0.0f}
	};
	//UV座標
	Transform2D uvTransform2D_ = {
		.size = { 1.0f,1.0f },
		.rotation = 0.0f,
		.translation = {0.0f,0.0f}
	};
	Transform transform_ = {};//トランスフォームの情報
	Transform uvTransform_ = {};//UVトランスフォームの情報
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

