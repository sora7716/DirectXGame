#include <Windows.h>

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//出力ウィンドウへの文字の表示
	OutputDebugStringA("Hellow,DirectX!\n");
	return 0;
}