#pragma once
#include "BaseObjectCommon.h"
//前方宣言
class Camera;

/// <summary>
/// 3Dオブジェクトの共通部分
/// </summary>
class Object3dCommon :public BaseObjectCommon {
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns></returns>
	static Object3dCommon* GetInstance();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// デフォルトカメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetDefaultCamera(Camera* camera);

	/// <summary>
	/// デフォルトカメラのゲッター
	/// </summary>
	/// <returns>デフォルトカメラ</returns>
	Camera* GetDefaultCamera()const;
private://メンバ関数
	//コンストラクタの封印
	Object3dCommon() = default;
	//デストラクタの封印
	~Object3dCommon() = default;
	//コピーコンストラクタ禁止
	Object3dCommon(const Object3dCommon&) = delete;
	//代入演算子の禁止
	Object3dCommon operator=(const Object3dCommon&) = delete;
private://静的メンバ変数
	//インスタンス
	static inline Object3dCommon* instance = nullptr;
	//Finalizeをしたかどうかのフラグ
	static inline bool isFinalize = false;
private://メンバ変数
	Camera* defaultCamera_ = nullptr;
};

