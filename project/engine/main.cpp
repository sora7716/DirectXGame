#include "framework/GameSystem.h"
#include "base/D3DResourceLeakChecker.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakChecker leakChacker;//メモリーリークをチェック
	//ゲームシステムの生成
	std::unique_ptr<GameSystem> gameSystem = std::make_unique<GameSystem>();
	//ゲームシステムの初期化
	gameSystem->Initialize();
	//ウィンドウの✖ボタンが押されるまでループ
	while (true) {
		if (WinApi::GetInstance()->ProcesMessage()) {
			break;
		}
		//ゲームシステムの更新
		gameSystem->Update();
		//ゲームシステムの描画
		gameSystem->Draw();
	}
	//ゲームシステムの終了
	gameSystem->Finalize();
	return 0;
}