#pragma once
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#include <memory>
#include <wrl.h>
#include "engine/math/Vector2.h"
#include "engine/math/Vector3.h"
#include <cstdint>
//前方宣言
class WinApi;
class Camera;

//マウスのクリック位置
enum Clic {
	kLeft,
	kRight,
	kMiddle
};

/// <summary>
/// 入力
/// </summary>
class Input final {
public://エイリアステンプレート
	//namespace省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns>インスタンス</returns>
	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApp">ウィンドウズアプリケーション</param>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>押されてるか</returns>
	bool PressKey(BYTE keyNumber);

	/// <summary>
	/// キーを押下した瞬間をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>押した瞬間</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// キーを離した瞬間をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>離した瞬間</returns>
	bool ReleaseTriggerKey(BYTE keyNumber);

	/// <summary>
	/// マウスのボタンの押下をチェック
	/// </summary>
	/// <param name="mouseClicPos">マウスのボタン</param>
	/// <returns>押されてるか</returns>
	bool PressMouseButton(Clic mouseClicPos);

	/// <summary>
	/// マウスのボタンの押下した瞬間をチェック
	/// </summary>
	/// <param name="mouseClicPos">マウスのボタン</param>
	/// <returns>押した瞬間</returns>
	bool TriggerMouseButton(Clic mouseClicPos);

	/// <summary>
	/// マウスのボタンを話した瞬間をチェック
	/// </summary>
	/// <param name="mouseClicPos">マウスのボタン</param>
	/// <returns>離した瞬間</returns>
	bool ReleaseTriggerMouseButton(Clic mouseClicPos);

	/// <summary>
	/// マウスの移動量のゲッター
	/// </summary>
	/// <returns>マウスの移動量</returns>
	const Vector2Int GetMouseMoveAmount()const;

	/// <summary>
	/// マウスホイールの回転量のゲッター
	/// </summary>
	/// <returns>マウスホイールの回転量</returns>
	const int32_t GetWheelRotate()const;

	/// <summary>
	/// ワールド座標系のマウスの位置のゲッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <returns>ワールド座標系のマウスの位置</returns>
	const Vector3 GetWorldMousePosition(Camera*camera)const;

	/// <summary>
	/// スクリーン座標系のマウスの位置のゲッター
	/// </summary>
	/// <returns>スクリーン座標系のマウスの位置</returns>
	const Vector2Int GetMousePosition()const;
private://メンバ関数
	//コンストラクタの封印
	Input() = default;
	//デストラクタの封印
	~Input() = default;
	//コピーコンストラクタを禁止
	Input(const Input&) = delete;
	//代入演算子を禁止
	const Input& operator=(const Input&) = delete;

	/// <summary>
	/// キーボード入力の初期化
	/// </summary>
	void KeyboardInitialize();

	/// <summary>
	/// キーボード入力の更新
	/// </summary>
	void KeyboardUpdate();

	/// <summary>
	/// マウス入力の初期化
	/// </summary>
	void MouseInitialize();

	/// <summary>
	/// マウス入力の更新
	/// </summary>
	void MouseUpdate();
private://静的メンバ変数
	//インスタンス
	static inline Input* instance = nullptr;
	//Finalizeを読んだかどうか
	static inline bool isFinalize = false;
private://メンバ変数
	//ウィンドウズAPI
	WinApi* winApi_ = nullptr;
	//キーボードデバイス
	ComPtr<IDirectInputDevice8> keyboard_ = nullptr;
	//全キーの入力状態を取得する
	BYTE keys_[256] = {};
	BYTE preKeys_[256] = {};
	//マウス
	ComPtr<IDirectInputDevice8>mouse_ = nullptr;
	DIMOUSESTATE mouseState_ = {};
	DIMOUSESTATE preMouseState_ = {};
};

