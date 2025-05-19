#pragma once
#include "BaseObjectCommon.h"

/// <summary>
/// パーティクルの共通部分
/// </summary>
class ParticleCommon :public BaseObjectCommon {
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns></returns>
	static ParticleCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXBase">DirectXの基盤部分</param>
	void Initialize(DirectXBase* directXBase)override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
private://メンバ関数
	//コンストラクタの封印
	ParticleCommon() = default;
	//デストラクタの封印
	~ParticleCommon() = default;
	//コピーコンストラクタ禁止
	ParticleCommon(const ParticleCommon&) = delete;
	//代入演算子の禁止
	ParticleCommon operator=(const ParticleCommon&) = delete;
private://静的メンバ変数
	//インスタンス
	static inline ParticleCommon* instance = nullptr;
	//Finalizeをしたかどうかのフラグ
	static inline bool isFinalize = false;
};
