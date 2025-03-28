#pragma once
#include "engine/math/ResourceData.h"
#include "engine/math/func/Rendering.h"
#include <string>
#include <wrl.h>
#include <d3d12.h>
//前方宣言
class SpriteCommon;
class DirectXBase;
class Camera;
class Sprite;

/// <summary>
/// 2Dオブジェクト
/// </summary>
class Object2d{
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object2d() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Object2d() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// スプライトのセッター
	/// </summary>
	/// <param name="sprite">スプライト</param>
	void SetSprite(Sprite* sprite);

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
	/// 座標変換行列リソースの生成
	/// </summary>
	void CreateTransformationMatrixResorce();

	/// <summary>
	/// 光源の生成
	/// </summary>
	void CreateDirectionLight();

	/// <summary>
    /// 座標変換の更新
    /// </summary>
	void UpdateTransform();
private://メンバ変数
	//ローカル座標
	Transform transform_ = {};
	//ローカル座標
	Transform2D transform2D_ = {
		.scale = { 90.0f,90.0f },
		.rotate = 0.0f,
		.translate = {0.0f,0.0f}
	};
	//UV座標
	Transform2D uvTransform_ = {
		.scale = { 1.0f,1.0f },
		.rotate = 0.0f,
		.translate = {0.0f,0.0f}
	};
	//カメラ
	Camera* camera_ = nullptr;
	//スプライトの共通部分
	SpriteCommon* spriteCommon_ = nullptr;
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//バッファリソース
	ComPtr<ID3D12Resource>wvpResoruce_ = nullptr;//座標変換行列
	ComPtr<ID3D12Resource>directionalLightResource_ = nullptr;//光源
	//バッファリソース内のデータを指すポインタ
	DirectionalLight* directionalLightData_ = nullptr;
	TransformationMatrix* wvpData_ = nullptr;
	Sprite* sprite_ = nullptr;
};

