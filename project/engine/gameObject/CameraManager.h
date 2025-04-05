#pragma once
#include "Camera.h"
#include <map>
#include <string>
#include <memory>

/// <summary>
/// カメラの管理
/// </summary>
class CameraManager {
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns>インスタンス</returns>
	static CameraManager* GetInstance();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// カメラを追加
	/// </summary>
	/// <param name="name">カメラの名前</param>
	void AddCamera(const std::string& name);

	/// <summary>
	/// カメラを検索
	/// </summary>
	/// <param name="name">カメラの名前</param>
	/// <returns>カメラ</returns>
	Camera* FindCamera(const std::string& name);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
private://メンバ関数
	//コンストラクタを封印
	CameraManager() = default;

	//デストラクタを封印
	~CameraManager() = default;

	//コピーコンストラクタ禁止
	CameraManager(const CameraManager&) = delete;
	//代入演算子を禁止
	CameraManager operator=(const CameraManager&) = delete;
private://静的メンバ変数
	//カメラのインスタンス
	static inline CameraManager* instance = nullptr;
	//終了処理をしたかのフラグ
	static inline bool isFialize = false;
private://メンバ変数
	//カメラ
	std::map<std::string, std::unique_ptr<Camera>>cameras_;
};

