#pragma once

/// <summary>
/// モデルの管理
/// </summary>
class ModelManager{
public://メンバ関数
private://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ModelManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ModelManager() = default;

	//コピーコンストラクタ禁止
	ModelManager(const ModelManager&) = delete;
	//代入演算子禁止
	ModelManager operator=(const ModelManager&) = delete;
private://静的メンバ変数
	static ModelManager* instance;
private://メンバ変数
};

