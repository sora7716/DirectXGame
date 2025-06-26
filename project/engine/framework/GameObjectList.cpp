#include "GameObjectList.h"
#include <cassert>

//インスタンスのゲッター
GameObjectList* GameObjectList::GetInstance() {
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new GameObjectList();
	}
	return instance;
}

//初期化
void GameObjectList::Initialize() {
	//管理を記録する
	audioManager_ = AudioManager::GetInstance();//オーディオ
	modelManager_ = ModelManager::GetInstance();//モデル
	cameraManager_ = CameraManager::GetInstance();//カメラ
	//オーディオの読み込み
	LoadAudio();
	//スプライトの読み込み
	LoadSprite();
	//OBJファイルの読み込み
	LoadModel();
	//カメラの生成
	CreateCamera();
}

//終了
void GameObjectList::Finalize() {
	delete instance;
	instance = nullptr;
	isFinalize = true;
}


//オーディオの読み込み
void GameObjectList::LoadAudio() {
	AudioManager::GetInstance()->LoadAudio("Alarm01", "Alarm01");
	AudioManager::GetInstance()->LoadAudio("mokugyo", "mokugyo");
}

//スプライトの読み込み
void GameObjectList::LoadSprite() {
}

//OBJファイルの読み込み
void GameObjectList::LoadModel() {
	//モデルの読み込み
	ModelManager::GetInstance()->LoadOBJModel("cube", "cube", "cube");
	//平面
	ModelManager::GetInstance()->LoadOBJModel("plane", "plane", "plane");
	//軸
	ModelManager::GetInstance()->LoadOBJModel("axis", "base", "axis");
}

//カメラの生成
void GameObjectList::CreateCamera() {
	//カメラの管理
	CameraManager::GetInstance()->AddCamera("defaultCamera");
	//タイトルカメラ
	CameraManager::GetInstance()->AddCamera("titleCamera");
}