#pragma once
#include <map>
#include <memory>
#include "engine/3d/ModelCommon.h"
#include "engine/3d/Model.h"

/// <summary>
/// モデルの管理
/// </summary>
class ModelManager {
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns>インスタンス</returns>
	static ModelManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXBase">DirectXの基盤部分</param>
	void Initialize(DirectXBase* directXBase);

	/// <summary>
	/// objモデルの読み込み
	/// </summary>
	/// <param name="name">名前</param>
    /// <param name="storedFilePath">保管しているファイル名("/"は書かなくていい)</param>
	/// <param name="filePath">ファイル名("/"と".obj"は書かなくていい)</param>
	void LoadOBJModel(const std::string& name, const std::string& storedFilePath, const std::string& filePath);

	/// <summary>
	/// モデルの検索(.objはいらない)
	/// </summary>
	/// <param name="name">名前</param>
	/// <returns>モデル</returns>
	Model* FindModel(const std::string& name);

	/// <su	mmary>
	/// 終了
	/// </summary>
	void Finalize();
private://メンバ関数
	//コンストラクタを封印
	ModelManager() = default;

	//デストラクタを封印
	~ModelManager() = default;

	//コピーコンストラクタ禁止
	ModelManager(const ModelManager&) = delete;
	//代入演算子禁止
	ModelManager operator=(const ModelManager&) = delete;
private://静的メンバ変数
	//インスタンス
	static ModelManager* instance;
	//Finalize()を呼んだかどうかのフラグ
	static inline bool isFinalize = false;
private://メンバ変数
	//モデルデータコンテナ
	std::map<std::string, std::unique_ptr<Model>>models_;
	//モデルの共通部分
	ModelCommon* modelCommon_ = nullptr;
};

