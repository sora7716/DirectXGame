#include "ModelManager.h"
#include <cassert>
//インスタンスの初期化
ModelManager* ModelManager::instance = nullptr;

// インスタンスのゲッター
ModelManager* ModelManager::GetInstance() {
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new ModelManager();
	}
	return instance;
}

//初期化
void ModelManager::Initialize(DirectXBase* directXBase) {
	modelCommon_ = new ModelCommon();
	modelCommon_->Initialize(directXBase);
}

// objモデルの読み込み
void ModelManager::LoadOBJModel(const std::string& name, const std::string& storedFilePath, const std::string& filePath) {
	//読み込み済みならモデルを検索
	if (models_.contains(name)) {
		//読み込み済みなら早期return
		return;
	}
	//モデルの生成とファイル読み込み、初期化
	std::unique_ptr<Model>model = std::make_unique<Model>();
	model->Initialize(modelCommon_, "engine/resources/models", storedFilePath, filePath+".obj");

	//モデルをmapコンテナに格納する
	models_.insert(std::make_pair(name, std::move(model)));
}

//モデルの検索
Model* ModelManager::FindModel(const std::string& name) {
	//読み込み済みモデルを検索
	if (models_.contains(name)) {
		//読み込み済みモデルを戻り値としてreturn
		return models_.at(name).get();
	}
	//ファイル名一致なし
	return nullptr;
}

// 終了
void ModelManager::Finalize() {
	delete instance;
	instance = nullptr;
	isFinalize = true;
}
