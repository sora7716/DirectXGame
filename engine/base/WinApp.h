#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>

/// <summary>
/// ウィンドウズアプリケーション
/// </summary>
class WinApp{
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	WinApp() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WinApp() = default;

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
	void InitializeWindow();

public://静的メンバ変数
	static inline const int32_t kClientWidth = 1280;
	static inline const int32_t kClientHeight = 720;
	static inline const std::wstring label = L"CG2";
private://メンバ変数
};

