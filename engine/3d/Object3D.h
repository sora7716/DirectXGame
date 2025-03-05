#pragma once
#include "engine/math/ResourceData.h"
#include "engine/math/rendering/Rendering.h"
#include <vector>
#include <string>

//前方宣言
class Object3dManager;

//マテリアルデータ
typedef struct MaterialData {
	std::string textureFilePath;
}MaterialData;


//モデルデータの構造体
typedef struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
}ModelData;	

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d{
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3d() = default;
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Object3d() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="object3dManager">3Dオブジェクトの共通部分</param>
	void Initialize(Object3dManager* object3dManager);

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
	/// 頂点データの初期化
	/// </summary>
	void InitialilzeVertexData();

	/// <summary>
	/// インデックスの初期化
	/// </summary>
	void InitializeIndexData();

	/// <summary>
	/// マテリアルデータの初期化
	/// </summary>
	void InitialilzeMaterialData();

	/// <summary>
	/// 座標変換行列の初期化
	/// </summary>
	void InitializeTransformationMatrixData();

private://メンバ変数
	//3Dオブジェクトの共通部分
	Object3dManager* object3dManager_ = nullptr;	
	//Objファイルのデータ
	ModelData modelData_;
	//バッファリソース
	ComPtr<ID3D12Resource>vertexResource_ = nullptr;//頂点
	ComPtr<ID3D12Resource>indexResource_ = nullptr;//インデックス
	ComPtr<ID3D12Resource>materialResource_ = nullptr;//マテリアル
	ComPtr<ID3D12Resource>transformationMatrixResoruce_ = nullptr;//座標変換行列
	ComPtr<ID3D12Resource>directionalLightResource_ = nullptr;//光源
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;//頂点
	uint32_t* indexData_ = nullptr;//インデックス
	Material* materialData_ = nullptr;//マテリアル
	DirectionalLight* directionalLightData_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};//頂点
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};//インデックス
};

