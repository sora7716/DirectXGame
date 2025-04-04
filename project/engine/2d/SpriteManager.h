#pragma once
#include <string>
#include <map>
#include <memory>

//前方宣言
class DirectXBase;
class Sprite;

/// <summary>
/// スプライトの管理
/// </summary>
class SpriteManager{
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns>インスタンス</returns>
	static SpriteManager* GetInstance();

	/// <summary>
	/// スプライトの読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadSprite(const std::string& filePath);

	/// <summary>
	/// スプライトの検索
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	Sprite* FindSprite(const std::string& filePath);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
private://メンバ関数
	//コンストラクタの封印
	SpriteManager() = default;
	//デストラクタの封印
	~SpriteManager() = default;
	//コピーコンストラクタの禁止
	SpriteManager(const SpriteManager&) = delete;
	//代入演算子の禁止
	SpriteManager operator=(const SpriteManager&) = delete;
private://静的メンバ変数
	//インスタント
	static inline SpriteManager* instance=nullptr;
	//終了処理をしたかどういか
	static inline bool isFinalize = false;
private://メンバ変数
	//スプライトたち
	std::map<std::string, std::unique_ptr<Sprite>>sprites_;
};

