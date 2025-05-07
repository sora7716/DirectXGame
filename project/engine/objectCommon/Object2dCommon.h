#pragma once
#include "BaseObjectCommon.h"

/// <summary>
/// 2Dオブジェクトの共通部分
/// </summary>
class Object2dCommon:public BaseObjectCommon{
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns></returns>
	static Object2dCommon* GetInstance();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
private://メンバ関数
	//コンストラクタの封印
	Object2dCommon() = default;
	//デストラクタの封印
	~Object2dCommon() = default;
	//コピーコンストラクタ禁止
	Object2dCommon(const Object2dCommon&) = delete;
	//代入演算子の禁止
	Object2dCommon operator=(const Object2dCommon&) = delete;
private://静的メンバ変数
	//インスタンス
	static inline Object2dCommon* instance = nullptr;
	//Finalizeをしたかどうかのフラグ
	static inline bool isFinalize = false;
};

