#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>

/// <summary>
/// ウィンドウズAPI
/// </summary>
class WinApi {
public://メンバ関数
	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns></returns>
	static WinApi* GetInstance();
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// プロセスメッセージ
	/// </summary>
	/// <returns>プロセスメッセージ</returns>
	bool ProcesMessage();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// HWNDのゲッター
	/// </summary>
	/// <returns>HWND</returns>
	HWND GetHwnd()const;

	/// <summary>
	/// WNDクラスのゲッター
	/// </summary>
	/// <returns>wndClass</returns>
	WNDCLASS GetWndClass()const;

	//コピーコンストラクタ禁止
	WinApi(const WinApi&) = delete;
	//代入演算子禁止
	const WinApi& operator=(const WinApi&) = delete;
private://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	WinApi() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WinApi() = default;
public://静的メンバ関数
	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">メッセージが送信されたウィンドウのハンドル</param>
	/// <param name="msg">メッセージの識別子</param>
	/// <param name="wparam">メッセージの追加情報</param>
	/// <param name="lparam">メッセージの追加情報</param>
	/// <returns></returns>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public://定数
	static inline const int32_t kClientWidth = 1280;//画面の横幅
	static inline const int32_t kClientHeight = 720;//画面の縦幅
	static inline const std::wstring label = L"CG2";//タイトル名
private://静的メンバ変数
	//インスタンス
	static inline WinApi* instance = nullptr;
	//Finalizeを読んだかどうか
	static inline bool isFinalize = false;
private://メンバ変数
	WNDCLASS wndClass_{};	//ウィンドウクラス
	HWND hwnd_ = nullptr;	//ウィンドウハンドル
	RECT windowRect_{};
};

