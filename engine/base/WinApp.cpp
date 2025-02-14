#include "WinApp.h"

//ウィンドウプロシージャ
LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)){
		return true;
	}
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
}

// ウィンドウの生成するための初期化
void WinApp::CreateGameWindow(){
	//ウィンドウプロシージャ
	wndClass_.lpfnWndProc = WindowProc;
	//ウィンドウのクラス名
	wndClass_.lpszClassName = L"CG2WindowClass";
	//インスタンスハンドル
	wndClass_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wndClass_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wndClass_);

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	windowRect_ = { 0,0,kClientWidth,kClientHeight };

	//クライアント領域を元に実際のサイズをwrcを変更してもらう
	AdjustWindowRect(&windowRect_, WS_OVERLAPPEDWINDOW, false);

	hwnd_ = CreateWindow(
		wndClass_.lpszClassName,//利用するクラス
		label.c_str(),
		WS_OVERLAPPEDWINDOW,//よく見るウィンドウのスタイル
		CW_USEDEFAULT,//表示X座標(Windowに任せる)
		CW_USEDEFAULT,//表示Y座標(Windowに任せる)
		windowRect_.right - windowRect_.left,//ウィンドウの横幅
		windowRect_.bottom - windowRect_.top,//ウィンドウの縦幅
		nullptr,
		nullptr,
		wndClass_.hInstance,//インスタンスハンドル
		nullptr
	);
	//ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);
}

// プロセスメッセージ
bool WinApp::ProcesMessage(){
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	if (msg.message == WM_QUIT) {
		return true;
	}
	return false;
}

//HWNDのゲッター
HWND WinApp::GetHwnd(){
	return hwnd_;
}

//WNDクラスのゲッター
WNDCLASS WinApp::GetWndClass(){
	return wndClass_;
}

//デストラクタ
WinApp::~WinApp(){
	CloseWindow(hwnd_);
}
