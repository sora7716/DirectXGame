#include "SceneManager.h"
#include <cassert>

//インストールしてものゲッター
SceneManager* SceneManager::GetInstance(){
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new SceneManager();
	}
	return instance;
}

//初期化
void SceneManager::Initialize(DirectXBase* directXBase){
	directXBase_ = directXBase;
}

//更新
void SceneManager::Update() {
	//次のシーンの予約があるなら
	if (nextScene_) {
		//旧シーンの終了
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}
		//シーンの切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;

		//シーンの管理をセット
		scene_->SetSeceneManager(this);

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

void SceneManager::Finalize(){
	scene_->Finalize();
	delete scene_;
	delete instance;
	instance = nullptr;
	isFinalize = true;
}

//次のシーンのセッター
void SceneManager::SetNextScene(IScene* nextScene) {
	nextScene_ = nextScene;
}
