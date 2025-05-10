#include "SpriteManager.h"
#include "Sprite.h"
#include <cassert>

//インスタンスのゲッター
SpriteManager* SpriteManager::GetInstance() {
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance==nullptr) {
		instance = new SpriteManager();
	}
	return instance;
}

//スプライトの読み込み
void SpriteManager::LoadSprite(const std::string& name, const std::string& filePath) {
	//読み込み済みならモデルを検索
	if (sprites_.contains(name)) {
		//読み込み済みなら早期return
		return;
	}
	//モデルの生成とファイル読み込み、初期化
	std::unique_ptr<Sprite>sprite = std::make_unique<Sprite>();
	sprite->Initialize("engine/resources/textures/"+filePath+".png");

	//モデルをmapコンテナに格納する 
	sprites_.insert(std::make_pair(name, std::move(sprite)));
}

//スプライトの検索
Sprite* SpriteManager::FindSprite(const std::string& name) {
	//読み込み済みモデルを検索
	if (sprites_.contains(name)) {
		//読み込み済みモデルを戻り値としてreturn
		return sprites_.at(name).get();
	}
	//ファイル名一致なし
	return nullptr;
}

//終了
void SpriteManager::Finalize() {
	delete instance;
	instance = nullptr;
	isFinalize = true;
}
