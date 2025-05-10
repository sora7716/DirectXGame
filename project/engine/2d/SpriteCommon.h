#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <string>
//前方宣言
class DirectXBase;

/// <summary>
/// スプライトの共通部分
/// </summary>
class SpriteCommon {
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns></returns>
	static SpriteCommon* GetInstance();

	/// <summary>
    /// 初期化
    /// </summary>
    /// <param name="directXBase">DirectXの基盤部分</param>
	void Initialize(DirectXBase* directXBase);

	/// <summary>
	/// テクスチャの読み込み
	/// </summary>
	/// <param name="filename">ファイル名</param>
	void LoadTexture(const std::string& filename);

	/// <summary>
	/// DirectXの基盤部分のゲッター
	/// </summary>
	/// <returns></returns>
	DirectXBase* GetDirectXBase();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
private://メンバ関数
	//コンストラクタを封印
	SpriteCommon() = default;
	//デストラクタを封印
	~SpriteCommon() = default;
	//コピーコンストラクタ禁止
	SpriteCommon(const SpriteCommon&) = delete;
	//代入演算子の禁止
	SpriteCommon operator=(const SpriteCommon&) = delete;
private://静的メンバ変数
	//インスタンス
	static inline SpriteCommon* instance = nullptr;
	//Finalizeしたかどうかのフラグ
	static inline bool isFinalize = false;
private://メンバ変数
	DirectXBase* directXBase_ = nullptr;
};

