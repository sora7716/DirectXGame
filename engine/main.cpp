#include "base/DirectXCommon.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//ウィンドウの作成
	WinApp* winApp = WinApp::GetInstance();
	winApp->CreateGameWindow();
	//ログ
	Log*log = Log::GetInstance();
	log->ConsolePrintf("Hellow,DirectX!\n");
	
	//DirectX12の初期化
	DirectXCommon::GetInstance()->InitializeDirectX12();

	//ウィンドウの×ボタンが押されるまでループ
	while (true){
		if (winApp->ProcesMessage()) {
			break;
		}
		//ゲームの処理
	}
	return 0;
}