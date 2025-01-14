#include "base/WinApp.h"
#include "2d/Log.h"
#include <memory>

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//ウィンドウの作成
	std::unique_ptr<WinApp>winApp = std::make_unique<WinApp>();
	winApp->InitializeWindow();
	//ログ
	std::unique_ptr<Log>log = std::make_unique<Log>();
	log->ConsolePrintf("Hellow,DirectX!\n");
	//メインループ
	MSG msg{};
	//ウィンドウの×ボタンが押されるまでループ
	while (msg.message!=WM_QUIT){
		//Windowにメッセージが来ていたら最優先で処理をさせる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else {
			// ゲームの処理
		}
	}
	return 0;
}