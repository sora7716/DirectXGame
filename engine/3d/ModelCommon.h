#pragma once
//前方宣言
class DirectXBase;

//3dモデルの共通部分
class ModelCommon{
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ModelCommon() = default;
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ModelCommon() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXBase">DirectXの基盤部分</param>
	void Initialize(DirectXBase* directXBase);

	/// <summary>
	/// DirectXの基盤部分のゲッター
	/// </summary>
	/// <returns></returns>
	DirectXBase* GetDirectXBase()const;
private://メンバ変数
	DirectXBase* directXBase_ = nullptr;
};