#pragma once
#include "IScene.h"
#include "AbstractSceneFactory.h"

/// <summary>
/// シーン管理
/// </summary>
class SceneManager {
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns>インスタンス</returns>
	static SceneManager* GetInstance();

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

	/// <summary>
	/// シーンファクトリーのセッター
	/// </summary>
	/// <param name="sceneFactory">シーンファクトリー</param>
	void SetSceneFactory(AbstractSceneFactory*sceneFactory);

	/// <summary>
	/// シーン切り替え
	/// </summary>
	/// <param name="sceneName"></param>
	void ChangeScene(const std::string& sceneName);
private://メンバ関数
	//コンストラクタの封印
	SceneManager() = default;
	//デストラクタの封印
	~SceneManager() = default;
	//コピーコンストラクタを禁止
	SceneManager(const SceneManager&) = delete;
	//代入演算子を禁止
	SceneManager operator=(const SceneManager&) = delete;
private://静的メンバ変数
	//インスタンス
	static inline SceneManager* instance = nullptr;
	//Finalizeをしたかどうか
	static inline bool isFinalize = false;
private://メンバ変数
	//シーン
	IScene* scene_ = nullptr;
	//次のシーン
	IScene* nextScene_ = nullptr;
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;
};

