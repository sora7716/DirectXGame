#pragma once
#include <map>
#include <memory>
#include "engine/3d/ModelCommon.h"
#include "engine/3d/Model.h"

/// <summary>
/// モデルの管理
/// </summary>
class ModelManager{
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
	/// モデルの読み込み
	/// </summary>
	/// <param name="storedFilePath">保管しているファイル名(最初と最後に"/"入らない)</param>
	/// <param name="filePath">ファイル名(最初に"/"いらない)</param>
	void LoadModel(const std::string& storedFilePath, const std::string& filePath);

	/// <summary>
	/// モデルの検索
	/// </summary>
	/// <param name="filePath">モデルファイルパス</param>
	/// <returns>モデル</returns>
	Model* FindModel(const std::string& filePath);

	/// <su	mmary>
	/// 終了
	/// </summary>
	void Finalize();
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

