#pragma once
#include "engine/math/func/Rendering.h"
//前方宣言
class DirectXBase;
class Object2d;

/// <summary>
/// パーティクルのエミット
/// </summary>
class ParticleEmit{
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParticleEmit() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParticleEmit() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXBase">DirectXの基盤部分</param>
	void Initialize(DirectXBase* directXBase);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
private://メンバ変数
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//オブジェクト2d
	Object2d* object2d_ = nullptr;
	//ワールドトランスフォーム
	Transform2D worldTransform_ = {};
};

