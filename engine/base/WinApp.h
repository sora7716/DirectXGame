#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// ウィンドウズアプリケーション
/// </summary>
class WinApp {
public://メンバ関数
	WinApp() = default;
	~WinApp();
	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">メッセージが送信されたウィンドウのハンドル</param>
	/// <param name="msg">メッセージの識別子</param>
	/// <param name="wparam">メッセージの追加情報</param>
	/// <param name="lparam">メッセージの追加情報</param>
	/// <returns></returns>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// ウィンドウの生成するための初期化
	/// </summary>
	void CreateGameWindow();

	/// <summary>
	/// プロセスメッセージ
	/// </summary>
	/// <returns>プロセスメッセージ</returns>
	bool ProcesMessage();


	/// <summary>
	/// HWNDのゲッター
	/// </summary>
	/// <returns>HWND</returns>
	HWND GetHwnd() { return hwnd_; }

private://メンバ関数
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

public://静的メンバ変数
	static inline const int32_t kClientWidth = 1280;//画面の横幅
	static inline const int32_t kClientHeight = 720;//画面の縦幅
	static inline const std::wstring label = L"CG2";//タイトル名
private://メンバ変数
	WNDCLASS wndClass_{};	//ウィンドウクラス
	HWND hwnd_ = nullptr;	//ウィンドウハンドル
	RECT windowRect_{};
};

