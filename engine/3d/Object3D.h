#pragma once
#include "engine/math/ResourceData.h"
#include "engine/math/rendering/Rendering.h"
#include <vector>
#include <string>
#include <wrl.h>
#include <d3d12.h>

//前方宣言
class Object3dManager;
class DirectXBase;

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
	/// <param name="directoryPath">ディレクトリファイル</param>
	/// <param name="filename">ファイル名</param>
	void Initialize(Object3dManager* object3dManager, const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

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

	/// <summary>
	/// 座標変換行列データの生成
	/// </summary>
	void CreateTransformationMatrixData();

	/// <summary>
	/// 光源の生成
	/// </summary>
	void CreateDirectionLight();

private://メンバ変数
	//ローカル座標
	Transform transform_ = {};
	// カメラ座標
	Transform cameraTransform_ = {};
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//3Dオブジェクトの共通部分
	Object3dManager* object3dManager_ = nullptr;	
	//Objファイルのデータ
	ModelData modelData_;
	//バッファリソース
	ComPtr<ID3D12Resource>vertexResource_ = nullptr;//頂点
	ComPtr<ID3D12Resource>materialResource_ = nullptr;//マテリアル
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;//座標変換行列
	ComPtr<ID3D12Resource> directionalLightResource_=nullptr;//光源
	ComPtr<ID3D12Resource>indexResource_ = nullptr;//インデックス
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;//頂点
	Material* materialData_ = nullptr;//マテリアル
	TransformationMatrix* wvpData_ = nullptr;//座標変換行列
	DirectionalLight* directionalLightData_ = nullptr;//光源
	uint32_t* indexData_ = nullptr;//インデックス
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};//頂点
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};//インデックス
};

