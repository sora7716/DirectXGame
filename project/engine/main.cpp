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

	//テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("engine/resources/texture/monsterBall.png");
	//スプライト
	std::vector<Vector2> pos;
	std::vector<Sprite*>sprites;
	for (uint32_t i = 0; i < 5; i++) {
		Sprite* sprite = new Sprite();
		sprite->Initialize(spriteCommon.get(), "engine/resources/texture/uvChecker.png");
		pos.push_back(Vector2(120.0f, 0.0f) * (float)i);
		sprite->SetTranslate(pos[i]);
		sprite->SetScale({ 100.0f,180.0f });
		sprites.push_back(sprite);
	}
	//モデルの読み込み
	ModelManager::GetInstance()->LoadModel("base", "axis.obj");
	std::vector<Transform>object3dTransforms;
	std::vector<Object3d*>object3ds;
	for (uint32_t i = 0; i < 2; i++) {
		Object3d* object3d = new Object3d();
		object3d->Initialize(object3dCommon.get());
		object3d->SetModel("axis.obj");
		Transform transform;
		transform.scale = { 1.0f,1.0f,1.0f };
		transform.rotate = {};
		transform.translate = { (float)i * -2.7f,0.0f,0.0f };
		object3dTransforms.push_back(transform);
		object3ds.push_back(object3d);
	}
	//音声読み込み
	AudioManager::GetInstance()->LoadAudio("mokugyo", "mokugyo.wav");
	//音声の再生
	AudioManager::GetInstance()->FindAudio("mokugyo")->SoundPlayWave(true);
	float volume = 1.0f;

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

		input->Update();

		/*if (input->PressKey(DIK_W)) {
			transform.translate.y += 0.01f;
		}
		else if (input->PressKey(DIK_S)) {
			transform.translate.y -= 0.01f;
		}
		if (input->PressKey(DIK_D)) {
			transform.translate.x += 0.01f;
		}
		else if (input->PressKey(DIK_A)) {
			transform.translate.x -= 0.01f;
		}*/

		//スプイライト用の更新
		for (uint32_t i = 0; i < sprites.size(); i++) {
			if (i % 2 == 0) {
				sprites[i]->ChangeTexture("engine/resources/texture/uvChecker.png");
			}
			else {
				sprites[i]->ChangeTexture("engine/resources/texture/monsterBall.png");
			}
			sprites[i]->SetTranslate(pos[i]);
			sprites[i]->Update();
		}
		for (uint32_t i = 0; i < object3ds.size(); i++) {
			object3ds[i]->SetTranslate(object3dTransforms[i].translate);
			object3dTransforms[i].rotate += 0.1f;
			object3ds[i]->SetRotate(object3dTransforms[i].rotate);
			object3ds[i]->Update();
		}
		//カメラの更新
		CameraManager::GetInstance()->Update();
		CameraManager::GetInstance()->FindCamera("defaultCamera")->SetTranslate(cameraTransform.translate);
		CameraManager::GetInstance()->FindCamera("defaultCamera1")->SetTranslate(cameraTransform1.translate);
		object3ds[0]->SetCamera(CameraManager::GetInstance()->FindCamera("defaultCamera1"));
		object3ds[1]->SetCamera(CameraManager::GetInstance()->FindCamera("defaultCamera"));
		/*if (input->TriggerKey(DIK_W)) {
			for (uint32_t i = 0; i < object3ds.size(); i++) {
				object3ds[i]->SetCamera(CameraManager::GetInstance()->FindCamera("defaultCamera"));
			}
		}
		else if (input->TriggerKey(DIK_S)) {
			for (uint32_t i = 0; i < object3ds.size(); i++) {
				object3ds[i]->SetCamera(CameraManager::GetInstance()->FindCamera("defaultCamera1"));
			}
		}*/
#ifdef USE_IMGUI
		ImGui::Begin("sound");
		ImGui::DragFloat("volume", &volume, 0.01f, 0.0f, 2.0f);
		ImGui::End();
#endif // USE_IMGUI

		/*ImGui::Begin("sprite");
		ImGui::DragFloat2("translate[0]", &pos[0].x, 0.1f);
		ImGui::DragFloat2("translate[1]", &pos[1].x, 0.1f);
		ImGui::End();

		ImGui::Begin("object3d");
		ImGui::DragFloat2("translate[0]", &object3dTransforms[0].translate.x, 0.1f);
		ImGui::DragFloat2("translate[1]", &object3dTransforms[1].translate.x, 0.1f);
		ImGui::End();*/

		/*ImGui::Begin("camera");
		ImGui::DragFloat3("translate", &cameraTransform.translate.x, 0.1f);
		ImGui::DragFloat3("translate1", &cameraTransform1.translate.x, 0.1f);
		ImGui::End();*/
		AudioManager::GetInstance()->FindAudio("mokugyo")->SetVolume(volume);

		//ImGuiの内部コマンドを生成する
		imguiManager->End();
		//描画処理
		directXBase->PreDraw();
		srvManager->PreDraw();
		//描画するコマンドを積む
		//3Dオブジェクトの描画準備
		object3dCommon->DrawSetting();
		//スプライトの描画準備
		spriteCommon->DrawSetting();
		//3Dオブジェクトの描画準備
		object3dCommon->DrawSetting();

		//スプライトの描画するコマンドを積む
		for (auto sprite : sprites) {
			sprite->Draw();
		}
		//3Dオブジェクトの描画するコマンドを積む
		for (auto object3d : object3ds) {
			object3d->Draw();
		}
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
	//スプライトの終了
	for (auto sprite : sprites) {
		delete sprite;
	}
	sprites.clear();
	//3dObjectの終了
	for (auto object3d : object3ds) {
		delete object3d;
	}
	object3ds.clear();
	//カメラマネージャーの終了
	CameraManager::GetInstance()->Finalize();
	//テクスチャマネージャーの終了
	TextureManager::GetInstance()->Finalize();
	//モデルマネージャーの終了
	ModelManager::GetInstance()->Finalize();
	return 0;
}