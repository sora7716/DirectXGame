#include "WinApp.h"

//ウィンドウプロシージャ
LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
    //メッセージに応じてゲーム固有の処理を行う
    switch(msg){
    //ウィンドウが破棄された
    case WM_DESTROY:
        //OSに対して、アプリの終了を伝える
        PostQuitMessage(0);
        return 0;   
    }
    //標準のメッセージ処理を行う
    return DefWindowProc(hwnd, msg, wparam, lparam);
    return LRESULT();
}

// ウィンドウの生成するための初期化
void WinApp::InitializeWindow(){
	WNDCLASS wc{};
	//ウィンドウプロシージャ
	wc.lpfnWndProc = WindowProc;
	//ウィンドウのクラス名
	wc.lpszClassName = L"CG2WindowClass";
	//インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wc);

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0,kClientWidth,kClientHeight };

	//クライアント領域を元に実際のサイズをwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd = CreateWindow(
		wc.lpszClassName,//利用するクラス
		label.c_str(),
		WS_OVERLAPPEDWINDOW,//よく見るウィンドウのスタイル
		CW_USEDEFAULT,//表示X座標(Windowに任せる)
		CW_USEDEFAULT,//表示Y座標(Windowに任せる)
		wrc.right - wrc.left,//ウィンドウの横幅
		wrc.bottom - wrc.top,//ウィンドウの縦幅
		nullptr,
		nullptr,
		wc.hInstance,//インスタンスハンドル
		nullptr
	);
	//ウィンドウを表示する
	ShowWindow(hwnd, SW_SHOW);
}
