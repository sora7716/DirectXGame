#pragma once
#include "Vector2.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include <cstdint>
#include <string>
#include <vector>

//頂点データ
typedef struct VertexData {
	Vector4 position;//直行座標
	Vector2 texcoord;//UV座標
	Vector3 normal;//法線
}VertexData;

//マテリアル
typedef struct Material {
	Vector4 color;//色
	int32_t enableLighting;//ライティングするかどうかのフラグ
	float padding[3];
	Matrix4x4 uvTransform;//UVTransform
}Material;

//平行光源
typedef struct DirectionalLight {
	Vector4 color;//ライトの色
	Vector3 direction;//ライトの向き
	float intensity;//輝度
}DirectionalLight;

//マテリアルデータ
typedef struct MaterialData {
	std::string textureFilePath;
	uint32_t srvIndex;
}MaterialData;


//モデルデータの構造体
typedef struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
}ModelData;