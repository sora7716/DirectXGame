#pragma once
#include <string>
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include "engine/math/ResourceData.h"
//マテリアルデータ
typedef struct MaterialData {
	std::string textureFilePath;
	uint32_t textureIndex = 0;
}MaterialData;


//モデルデータの構造体
typedef struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
}ModelData;

//前方宣言
class ModelCommon;
class DirectXBase;

//3dモデル
class Model{
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Model() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Model() = default;
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelCommon">モデルの共通部分</param>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="filename">ファイル名</param>
	void Initialize(ModelCommon*modelCommon, const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
    /// .mtlファイルの読み取り	
    /// </summary>
    /// <param name="directoryPath">ディレクトリファイルパス</param>
    /// <param name="filename">ファイル名</param>
    /// <returns>マテリアルデータ</returns>
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// .objファイル読み取り
	/// </summary>
	/// <param name="directoryPath">ディレクトリファイルパス</param>
	/// <param name="filename">ファイル名</param>
	/// <returns>モデルデータ</returns>
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

private://メンバ関数
	/// <summary>
	/// 頂点データの生成
	/// </summary>
	void CreateVertexData();

	/// <summary>
	/// インデックスデータの生成
	/// </summary>
	void CreateIndexData();

	/// <summary>
	/// マテリアルデータの生成
	/// </summary>
	void CreateMaterialData();
private://メンバ変数
	//ModelCommonのポインタ
	ModelCommon* modelCommon_ = nullptr;
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//Objファイルデータ
	ModelData modelData_ = {};
	//VertexResource
	ComPtr<ID3D12Resource>vertexResource_ = nullptr;
	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	//VertexResourceにデータを書き込むためのポインタ
	VertexData* vertexData_ = nullptr;
	//マテリアルリソース
	ComPtr<ID3D12Resource>materialResource_ = nullptr;
	//マテリアルリソースにデータを書き込むためのポインタ
	Material* materialData_ = nullptr;
	//IndexResource
	ComPtr<ID3D12Resource>indexResource_ = nullptr;
	//IndexResourceにデータを書き込むためのポインタ
	uint32_t* indexData_ = nullptr;
	//IndexBufferView
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};//インデックス	
};

