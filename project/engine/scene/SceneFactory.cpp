#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
// シーンの生成
IScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//次のシーンの生成
	IScene* newScene = nullptr;
	if (sceneName == "Title") {
		newScene = new TitleScene();
	} else if (sceneName == "Game") {
		newScene = new GameScene();
	}
	return newScene;
}
