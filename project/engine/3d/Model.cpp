#include "Model.h"
#include "engine/base/DirectXBase.h"
#include "engine/3d/ModelCommon.h"
#include "engine/math/func/Math.h"
#include "engine/2d/TextureManager.h"
#include <cassert>
#include <fstream>
#include <sstream>

//初期化
void Model::Initialize(ModelCommon* modelCommon, const std::string& directoryPath, const std::string& storedFilePath, const std::string& filename) {
	//Nullチェック
	assert(modelCommon);
	//ModelCommonのポインタを引数からメンバ変数を記録する
	modelCommon_ = modelCommon;
	directXBase_ = modelCommon_->GetDirectXBase();
	//モデルの読み込み
	modelData_ = LoadObjFile(directoryPath, storedFilePath, filename);
	//頂点データの生成
	CreateVertexResource();
	//インデックスデータの生成
	CreateIndexResource();
	//マテリアルデータの生成
	CreateMaterialResource();
	//テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath );
	//テクスチャ番号を取得して、メンバ変数に書き込む
	//modelData_.material.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData_.material.textureFilePath);
}

//描画
void Model::Draw() {
	//VertexBufferViewの設定
	directXBase_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
	directXBase_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);//IBVを設定
	//マテリアルCBufferの場所を設定
	directXBase_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定
	directXBase_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVHandleGPU(modelData_.material.textureFilePath));
	//描画
	directXBase_->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.vertices.size()), 1, 0, 0, 0);
}

//uv変換
void Model::UVTransform(Transform2d uvTransform) {
	materialData_->uvTransform = Rendering::MakeUVAffineMatrix({ uvTransform.scale.x,uvTransform.scale.y,1.0f }, uvTransform.rotate, { uvTransform.translate.x,uvTransform.translate.y,1.0f });
}

// 色を変更
void Model::SetColor(const Vector4& color) {
	materialData_->color = color;
}

//テクスチャの変更
void Model::SetTexture(const std::string& filePath){
	TextureManager::GetInstance()->LoadTexture(filePath);
	modelData_.material.textureFilePath = filePath;
}

// 色を取得
const Vector4& Model::GetColor() const {
	// TODO: return ステートメントをここに挿入します
	return materialData_->color;
}

// .mtlファイルの読み取り	
MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	//1.中で必要となる変数の宣言
	MaterialData materialData;//構築するMaterialData
	std::string line;//ファイルから読んだ1行を格納するもの
	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);//ファイルを開く
	assert(file.is_open());//とりあえず開かなかったら止める
	//3.実際にファイルを読み込み、MaterialDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;
		//identifilerに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			//連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}
	//4.MaterialDataを返す
	return materialData;
}

// .objファイル読み取り
ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& storedFilePath, const std::string& filename) {
	//1.中で必要な変数を宣言
	ModelData modelData;//構築するModelData
	std::vector<Vector4>positions;//位置
	std::vector<Vector3>normals;//法線
	std::vector<Vector2>texcoords;//テクスチャ座標
	std::string line;//ファイルから呼んだ1桁を格納するもの
	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + storedFilePath + "/" + filename);//ファイルを開く
	assert(file.is_open());//とりあえず開かなかったら止める
	//3.実際にファイルを読み込み、ModelDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;//先頭の識別子を読む
		//identifierに応じた処理
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.x *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
			//面は三角形限定。そのほかは未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				//頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3]{};
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');// /区切りで読み込んでいく
					elementIndices[element] = std::stoi(index);
				}
				//要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				/*VertexData vertex = { position,texcoord,normal };
				modelData.vertices.push_back(vertex);*/
				triangle[faceVertex] = { position,texcoord,normal };
			}
			//頂点を逆順で登録することで、周りを逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			//materialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			//基本的にobjファイルと同一の階層にmtlは存在させるので、ディレクトリ名とファイル名を返す
			modelData.material = LoadMaterialTemplateFile(directoryPath + "/" + storedFilePath, materialFilename);
		}
	}
	//4.ModelDataを返す
	return modelData;
}


//頂点リソースの生成
void Model::CreateVertexResource() {
	//頂点リソースを生成
	vertexResource_ = directXBase_->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	//VertexBufferViewを作成する(頂点バッファービュー)
	//リソースの先頭アドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());//頂点データをリソースにコピー
}

//インデックスリソースの生成
void Model::CreateIndexResource() {
	//Index用(3dGameObject)
	indexResource_ = directXBase_->CreateBufferResource(sizeof(uint32_t) * modelData_.vertices.size());
	//リソースの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * modelData_.vertices.size());
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	//IndexResourceにデータを書き込む
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	for (int i = 0; i < modelData_.vertices.size(); i++) {
		indexData_[i] = i; indexData_[i + 1] = i + 1; indexData_[i + 2] = i + 2;
		indexData_[i + 3] = i + 1; indexData_[i + 4] = i + 3; indexData_[i + 5] = i + 2;
	}
}

//マテリアルリソースの生成
void Model::CreateMaterialResource() {
	//マテリアル用のリソースを作る
	materialResource_ = directXBase_->CreateBufferResource(sizeof(Material));
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//色を書き込む
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->enableLighting = true;
	materialData_->uvTransform = Math::MakeIdentity4x4();
}
