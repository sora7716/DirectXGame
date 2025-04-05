#include "CameraManager.h"
#include <cassert>

//インスタンスのゲッター
CameraManager* CameraManager::GetInstance(){
    assert(!isFialize && "GetInstance() called after Finalize()");
    if (instance == nullptr) {
        instance = new CameraManager();
    }
    return instance;
}

//更新
void CameraManager::Update(){
	for (auto& [name, camera] : cameras_) {
		camera->Update();
	}
}

//カメラの追加
void CameraManager::AddCamera(const std::string& name){
	//読み込み済みならカメラを検索
	if (cameras_.contains(name)) {
		//読み込み済みなら早期return
		return;
	}
	//カメラの生成
	std::unique_ptr<Camera>camera = std::make_unique<Camera>();

	//カメラをmapコンテナに格納する
	cameras_.insert(std::make_pair(name, std::move(camera)));
}

//カメラの検索
Camera* CameraManager::FindCamera(const std::string& name){
	//読み込み済みモデルを検索
	if (cameras_.contains(name)) {
		//読み込み済みモデルを戻り値としてreturn
		return cameras_.at(name).get();
	}
	//ファイル名一致なし
	return nullptr;
}

//終了
void CameraManager::Finalize(){
	delete instance;
	instance = nullptr;
	isFialize = true;
}
