#pragma once
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#include <memory>
#include <wrl.h>

//前方宣言
class WinApi;

/// <summary>
/// 入力
/// </summary>
class Input final{
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
	void Initialize(std::weak_ptr<WinApi>winApp);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

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
	/// <param name="keyNumber"></param>
	/// <returns>離した瞬間</returns>
	bool ReleaseTriggerKey(BYTE keyNumber);

	//コピーコンストラクタを禁止
	Input(const Input&) = delete;
	//代入演算子を禁止
	const Input& operator=(const Input&) = delete;
private://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Input() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Input() = default;
private://メンバ変数
	//ウィンドウズAPI
	std::shared_ptr<WinApi> winApi_ = nullptr;
	//キーボードデバイス
	ComPtr<IDirectInputDevice8> keyboard_ = nullptr;
	//全キーの入力状態を取得する
	BYTE key[256] = {};
	BYTE preKey[256] = {};
};

