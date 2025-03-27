#include "SceneManager.h"
#include <cassert>

//インスト�Eルしても�EゲチE��ー
SceneManager* SceneManager::GetInstance() {
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new SceneManager();
	}
	return instance;
}

//初期匁E
void SceneManager::Initialize(DirectXBase* directXBase) {
	directXBase_ = directXBase;
}

//更新
void SceneManager::Update() {
	//次のシーンの予紁E��あるなめE
	if (nextScene_) {
		//旧シーンの終亁E
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}
		//シーンの刁E��替ぁE
		scene_ = nextScene_;
		nextScene_ = nullptr;
		//次のシーン
		scene_->Initialize(directXBase_);
	}
	//更新
	scene_->Update();
}

//描画
void SceneManager::Draw() {
	//描画
	scene_->Draw();
}

void SceneManager::Finalize() {
	scene_->Finalize();
	delete scene_;
	delete instance;
	instance = nullptr;
	isFinalize = true;
}

//シーンファクトリーのセチE��ー
void SceneManager::SetSceneFactory(AbstractSceneFactory* sceneFactory) {
	sceneFactory_ = sceneFactory;
}

//シーンの刁E��替ぁE
void SceneManager::ChangeScene(const std::string& sceneName) {
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}
