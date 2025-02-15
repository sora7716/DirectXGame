#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// ウィンドウズAPI
/// </summary>
class WinApi {
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	WinApi() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WinApi() = default;

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
private://メンバ変数
	WNDCLASS wndClass_{};	//ウィンドウクラス
	HWND hwnd_ = nullptr;	//ウィンドウハンドル
	RECT windowRect_{};
};

