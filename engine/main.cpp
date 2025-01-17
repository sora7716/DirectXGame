#include "base/DirectXCommon.h"

struct D3DResourceLeakChecker {
	~D3DResourceLeakChecker() {
		Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
			debug->Release();
		}
	}
};

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakChecker leakCheck;
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
	return 0;
}