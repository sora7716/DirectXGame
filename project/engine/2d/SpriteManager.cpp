#include "SpriteManager.h"
#include "Sprite.h"
#include <cassert>

//インスタンスのゲッター
SpriteManager* SpriteManager::GetInstance() {
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance) {
		instance = new SpriteManager();
	}
	return instance;
}

//スプライトの読み込み
void SpriteManager::LoadSprite(const std::string& filePath) {
	//読み込み済みならモデルを検索
	if (sprites_.contains(filePath)) {
		//読み込み済みなら早期return
		return;
	}
	//モデルの生成とファイル読み込み、初期化
	std::unique_ptr<Sprite>sprite = std::make_unique<Sprite>();
	sprite->Initialize("engine/resources/"+filePath+".png");

	//モデルをmapコンテナに格納する
	sprites_.insert(std::make_pair(filePath, std::move(sprite)));
}

//スプライトの検索
void SpriteManager::FindSprite(const std::string& filePath) {
}

//終了
void SpriteManager::Finalize() {
	delete instance;
	instance = nullptr;
	isFinalize = true;
}
