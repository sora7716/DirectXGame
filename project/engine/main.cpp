#include "base/DirectXBase.h"
#include "2d/TextureManager.h"
#include "math/func/Math.h"
#include "audio/AudioManager.h"
#include "input/Input.h"
#include "base/D3DResourceLeakChecker.h"
#include "objectCommon/SpriteCommon.h"
#include "2d/Sprite.h"
#include "objectCommon/Object3dCommon.h"
#include "3d/Object3d.h"
#include "3d/ModelManager.h"
#include "3d/CameraManager.h"
#include "base/SRVManager.h"
#include"debug/ImGuiManager.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	HRESULT hr = S_FALSE;
	D3DResourceLeakChecker leakChacker;//メモリーリークをチェック
	//ウィンドウズアプリケーション
	WinApi* winApi = WinApi::GetInstance();
	//DirectXCommon
	std::unique_ptr<DirectXBase>directXBase = std::make_unique<DirectXBase>();
	//SRVマネージャー
	std::unique_ptr<SRVManager> srvManager = std::make_unique<SRVManager>();
	//ImGuiManagerの生成
	std::unique_ptr<ImGuiManager>imguiManager = std::make_unique<ImGuiManager>();
	//スプライトの共通部分
	std::unique_ptr<SpriteCommon>spriteCommon = std::make_unique<SpriteCommon>();
	//3Dオブジェクトの共通部分
	std::unique_ptr<Object3dCommon>object3dCommon = std::make_unique<Object3dCommon>();
	//ModelManagerの初期化
	ModelManager::GetInstance()->Initialize(directXBase.get());
	//ウィンドウの作成
	winApi->Initialize();
	//DirectX12の初期化
	directXBase->Initialize();
	//SRVマネージャーの初期化
	srvManager->Initialize(directXBase.get());
	//ImGuiManagerの初期化
	imguiManager->Initialize(winApi, directXBase.get(), srvManager.get());
	//テクスチャマネージャーの初期化
	TextureManager::GetInstance()->Initialize(directXBase.get(), srvManager.get());
	//カメラマネージャーの追加
	CameraManager::GetInstance()->AddCamera("defaultCamera");
	CameraManager::GetInstance()->AddCamera("defaultCamera1");
	//カメラ
	object3dCommon->SetDefaultCamera(CameraManager::GetInstance()->FindCamera("defaultCamera"));
	object3dCommon->SetDefaultCamera(CameraManager::GetInstance()->FindCamera("defaultCamera1"));
	Transform cameraTransform = { {},{},{0.0f,0.0f,-10.0f} };
	Transform cameraTransform1 = { {},{},{0.0f,0.0f,-100.0f} };

	//スプライトの共通部分の初期化
	spriteCommon->Initialize(directXBase.get());
	//3Dオブジェクトの共通部分の初期化
	object3dCommon->Initialize(directXBase.get());

	//スプライト
	std::unique_ptr<Sprite>sprite = std::make_unique<Sprite>();
	sprite->Initialize(spriteCommon.get(), "block.png");
	Vector2 spritePos = { 100.0f,100.0f };
	//音声読み込み
	AudioManager::GetInstance()->LoadAudio("mokugyo", "mokugyo.wav");
	//音声の再生
	AudioManager::GetInstance()->FindAudio("mokugyo")->SoundPlayWave(true);
	float volume = 1.0f;

	//スプライトの読み込み

	//入力関数の初期化処理
	//DirectInputの初期化
	Input* input = Input::GetInstance();
	input->Initialize();

	//ウィンドウの✖ボタンが押されるまでループ
	while (true) {
		if (winApi->ProcesMessage()) {
			break;
		}
		//ImGuiの開始
		imguiManager->Begin();
		//入力関数の更新
		input->Update();

		//カメラの更新
		CameraManager::GetInstance()->Update();
		CameraManager::GetInstance()->FindCamera("defaultCamera")->SetTranslate(cameraTransform.translate);
		CameraManager::GetInstance()->FindCamera("defaultCamera1")->SetTranslate(cameraTransform1.translate);

		//スプライトの更新
		sprite->SetTranslate(spritePos);
		sprite->Update();
#ifdef USE_IMGUI
		ImGui::Begin("sound");
		ImGui::DragFloat("volume", &volume, 0.01f, 0.0f, 2.0f);
		ImGui::End();

		ImGui::Begin("sprite");
		ImGui::SliderFloat2("translate", &spritePos.x, 0.1f, 1000.0f, "%5.1f");
		ImGui::End();
#endif // USE_IMGUI
		//音のボリュームをセット
		AudioManager::GetInstance()->FindAudio("mokugyo")->SetVolume(volume);

		//ImGuiの内部コマンドを生成する
		imguiManager->End();
		//描画処理
		directXBase->PreDraw();
		//SRVの描画処理
		srvManager->PreDraw();
		//描画するコマンドを積む
		//3Dオブジェクトの描画準備
		object3dCommon->DrawSetting();
		//スプライトの描画準備
		spriteCommon->DrawSetting();
		//3Dオブジェクトの描画準備
		object3dCommon->DrawSetting();
		//スプライトの描画
		sprite->Draw();
		//実際のcommandListのImGuiの描画コマンドを積む
		imguiManager->Draw();
		//描画の終了
		directXBase->PostDraw();
	}
	//終了処理
	//ImGuiの終了
	imguiManager->Finalize();
	//WindowsAPIの終了
	winApi->Finalize();
	//Audioの終了
	AudioManager::GetInstance()->Finalize();
	//カメラマネージャーの終了
	CameraManager::GetInstance()->Finalize();
	//テクスチャマネージャーの終了
	TextureManager::GetInstance()->Finalize();
	//モデルマネージャーの終了
	ModelManager::GetInstance()->Finalize();
	return 0;
}