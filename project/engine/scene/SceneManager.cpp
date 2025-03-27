#include "SceneManager.h"
#include <cassert>

//繧､繝ｳ繧ｹ繝医・繝ｫ縺励※繧ゅ・繧ｲ繝・ち繝ｼ
SceneManager* SceneManager::GetInstance() {
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new SceneManager();
	}
	return instance;
}

//蛻晄悄蛹・
void SceneManager::Initialize(DirectXBase* directXBase) {
	directXBase_ = directXBase;
}

//譖ｴ譁ｰ
void SceneManager::Update() {
	//谺｡縺ｮ繧ｷ繝ｼ繝ｳ縺ｮ莠育ｴ・′縺ゅｋ縺ｪ繧・
	if (nextScene_) {
		//譌ｧ繧ｷ繝ｼ繝ｳ縺ｮ邨ゆｺ・
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}
		//繧ｷ繝ｼ繝ｳ縺ｮ蛻・ｊ譖ｿ縺・
		scene_ = nextScene_;
		nextScene_ = nullptr;
		//谺｡縺ｮ繧ｷ繝ｼ繝ｳ
		scene_->Initialize(directXBase_);
	}
	//譖ｴ譁ｰ
	scene_->Update();
}

//謠冗判
void SceneManager::Draw() {
	//謠冗判
	scene_->Draw();
}

void SceneManager::Finalize() {
	scene_->Finalize();
	delete scene_;
	delete instance;
	instance = nullptr;
	isFinalize = true;
}

//繧ｷ繝ｼ繝ｳ繝輔ぃ繧ｯ繝医Μ繝ｼ縺ｮ繧ｻ繝・ち繝ｼ
void SceneManager::SetSceneFactory(AbstractSceneFactory* sceneFactory) {
	sceneFactory_ = sceneFactory;
}

//繧ｷ繝ｼ繝ｳ縺ｮ蛻・ｊ譖ｿ縺・
void SceneManager::ChangeScene(const std::string& sceneName) {
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}
