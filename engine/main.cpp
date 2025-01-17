#include "base/DirectXCommon.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//ログ
	Log* log = Log::GetInstance();
	//ウィンドウズアプリケーション
	WinApp* winApp = WinApp::GetInstance();
	//DirectXCommon
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();

	//ウィンドウの作成
	winApp->CreateGameWindow();
	//デバックレイヤー
	directXCommon->DebugLayer();
	//DirectX12の初期化
	directXCommon->InitializeDirectX12();
	//ログの表示
	log->ConsolePrintf("Hellow,DirectX!\n");

	//ウィンドウの×ボタンが押されるまでループ
	while (true){
		if (winApp->ProcesMessage()) {
			break;
		}
		//ゲームの処理
		directXCommon->ChangeGameWindowColor();
	}
	directXCommon->ResourceLeakCheck();
	return 0;
}