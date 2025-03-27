#include "SceneManager.h"

//デストラクタ
SceneManager::~SceneManager(){
	scene_->Finalize();
	delete scene_;
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

//次のシーンのセッター
void SceneManager::SetNextScene(IScene* nextScene) {
	nextScene_ = nextScene;
}
