#pragma once
#include "engine/math/ResourceData.h"
#include "engine/math/func/Rendering.h"
#include "engine/gameObject/WorldTransform.h"
#include "engine/blend/BlendMode.h"
#include <string>
#include <wrl.h>
#include <d3d12.h>
#include <memory>
//前方宣言
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
	/// <param name="name">スプライトの名前</param>
	void SetSprite(const std::string& name);

	/// <summary>
	/// テクスチャの変更
	/// </summary>
	/// <param name="spriteName">スプライトの名前</param>
	void ChangeTexture(std::string spriteName);

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
	/// ワールドトランスフォームのゲッター
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	WorldTransform* GetWorldTransform();

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
	/// トランスフォームのセッター
	/// </summary>
	/// <param name="transform2d">トランスフォ-ム</param>
	void SetTransform(const Transform2d& transform2d);

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

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera);

	/// <summary>
	/// 親のセッター
	/// </summary>
	/// <param name="parent">親</param>
	void SetParent(const WorldTransform* parent);

	/// <summary>
	/// ブレンドモードセッター
	/// </summary>
	/// <param name="blendMode"></param>
	void SetBlendMode(const BlendMode& blendMode);
private://メンバ変数
	//UV座標
	Transform2d uvTransform_ = {
		.scale = { 1.0f,1.0f },
		.rotate = 0.0f,
		.translate = {0.0f,0.0f}
	};
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//スプライト
	Sprite* sprite_ = nullptr;
	//ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
	//ブレンドモード
	BlendMode blendMode_ = BlendMode::kNone;
};

