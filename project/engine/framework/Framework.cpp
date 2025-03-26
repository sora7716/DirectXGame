#include "Framework.h"
#include "engine/2d/TextureManager.h"

//初期化
void Framework::Initialize() {
	//winApi
	WinApi::GetInstance()->Initialize();
	//DirectXの基盤部分
	directXBase_ = std::make_unique<DirectXBase>();
	directXBase_->Initialize();
	//SRVの管理
	SRVManager::GetInstance()->Initialize(directXBase_.get());
	//入力処理
	input_ = Input::GetInstance();
	input_->Initialize();
	//テクスチャ管理
	TextureManager::GetInstance()->Initialize(directXBase_.get());
	//モデルの管理
	modelManager_ = ModelManager::GetInstance();
	modelManager_->Initialize(directXBase_.get());
	//ImGuiの管理
	ImGuiManager::GetInstance()->Initialize(directXBase_.get());
	//カメラの管理
	cameraManager_ = CameraManager::GetInstance();
	//オーディオの管理
	audioManager_ = AudioManager::GetInstance();
	//3Dオブジェクトの共通部分
	Object3dCommon::GetInstance()->Initialize(directXBase_.get());
	//スプライトの共通部分
	SpriteCommon::GetInstance()->Initialize(directXBase_.get());
}

//更新
void Framework::Update() {
	//入力処理
	input_->Update();
	//カメラの管理
	cameraManager_->Update();
}

//終了
void Framework::Finalize() {
	//winApi
	WinApi::GetInstance()->Finalize();
	//DirectXの基盤部分
	directXBase_->Finalize();
	//SRVの管理
	SRVManager::GetInstance()->Finalize();
	//入力
	input_->Finalize();
	//テクスチャ管理
	TextureManager::GetInstance()->Finalize();
	//モデルの管理
	modelManager_->Finalize();
	//ImGuiの管理
	ImGuiManager::GetInstance()->Finalize();
	//カメラの管理
	cameraManager_->Finalize();
	//オーディオの管理
	audioManager_->Finalize();
	//スプライトの共通部分
	SpriteCommon::GetInstance()->Finalize();
	//3Dオブジェクトの共通部分
	Object3dCommon::GetInstance()->Finalize();
}

//ゲームループ
void Framework::Run() {
	//ゲームシステムの初期化
	Initialize();
	//ウィンドウの✖ボタンが押されるまでループ
	while (isEndRequest()) {
		//ゲームシステムの更新
		Update();
		//ゲームシステムの描画
		Draw();
#ifdef _DEBUG
		//エスケイプを押したらループを抜ける
		if (input_->TriggerKey(DIK_ESCAPE) && input_->PressKey(DIK_LSHIFT)) {
			break;
		}
#endif // _DEBUG
	}
	//ゲームシステムの終了
	Finalize();
}

//終了リクエスト
bool Framework::isEndRequest() {
	return WinApi::GetInstance()->ProcesMessage();
}
