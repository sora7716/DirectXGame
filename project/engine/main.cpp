#include "framework/GameSystem.h"
#include "engine/base/D3DResourceLeakChecker.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//メモリーリークをチェック
	D3DResourceLeakChecker leakChacker;

	//ゲームシステムの生成
	std::unique_ptr<Framework> gameSystem = std::make_unique<GameSystem>();

	//ゲームループ
	gameSystem->Run();
	return 0;
}