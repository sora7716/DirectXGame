#include "Rendering.h"
#include "engine/math/func/Math.h"
using namespace std;

//拡縮
Matrix4x4 Rendering::MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result{
		scale.x,0.0f,0.0f,0.0f,
		0.0f,scale.y,0.0f,0.0f,
		0.0f,0.0f,scale.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

//平行移動
Matrix4x4 Rendering::MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		translate.x,translate.y,translate.z,1.0f
	};
	return result;
}

//同次座標系で計算しデカルト座標系に変換
Vector3 Rendering::TransformVector(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	//assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

//x座標を軸に回転
Matrix4x4 Rendering::MakeRotateXMatrix(const float& radian) {
	Matrix4x4 result{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cos(radian),sin(radian),0.0f,
		0.0f,-sin(radian),cos(radian),0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};
	return result;
}

//y座標を軸に回転
Matrix4x4 Rendering::MakeRotateYMatrix(const float& radian) {
	Matrix4x4 result{
		cos(radian),0.0f,-sin(radian),0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sin(radian),0.0f,cos(radian),0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

//z座標を軸に回転
Matrix4x4 Rendering::MakeRotateZMatrix(const float& radian) {
	Matrix4x4 result{
		cos(radian),sin(radian),0.0f,0.0f,
		-sin(radian),cos(radian),0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

//x,y,z座標で回転
Matrix4x4 Rendering::MakeRotateXYZMatrix(const Vector3& radian) {
	return MakeRotateXMatrix(radian.x) * MakeRotateYMatrix(radian.y) * MakeRotateZMatrix(radian.z);
}

// OBB用の回転行列
void Rendering::MakeOBBRotateMatrix(Vector3* orientations, const Vector3& rotate) {
	Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(rotate);

	//回転行列からの抽出

	//X'
	orientations[0].x = rotateMatrix.m[0][0];
	orientations[0].y = rotateMatrix.m[0][1];
	orientations[0].z = rotateMatrix.m[0][2];

	//Y'
	orientations[1].x = rotateMatrix.m[1][0];
	orientations[1].y = rotateMatrix.m[1][1];
	orientations[1].z = rotateMatrix.m[1][2];

	//Z'
	orientations[2].x = rotateMatrix.m[2][0];
	orientations[2].y = rotateMatrix.m[2][1];
	orientations[2].z = rotateMatrix.m[2][2];
}

// OBB用のワールド行列
Matrix4x4 Rendering::MakeOBBWorldMatrix(const Vector3* orientations, const Vector3 center) {
	Matrix4x4 result{
		orientations[0].x,orientations[0].y,orientations[0].z,0.0f,
		orientations[1].x,orientations[1].y,orientations[1].z,0.0f,
		orientations[2].x,orientations[2].y,orientations[2].z,0.0f,
		center.x,center.y,center.z,1.0f,
	};
	return result;
}

//アフィン関数
Matrix4x4 Rendering::MakeAffineMatrix(const Transform& transform) {
	return (MakeScaleMatrix(transform.scale) * MakeRotateXYZMatrix(transform.rotate)) * MakeTranslateMatrix(transform.translate);
}

//STRの変換
Matrix4x4 Rendering::MakeSTRMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	return MakeScaleMatrix(scale) * MakeTranslateMatrix(translate) * MakeRotateXYZMatrix(rotate);
}

// UVのアフィン変換
Matrix4x4 Rendering::MakeUVAffineMatrix(const Vector3& scale, float rotate, const Vector3& translate) {
	return MakeScaleMatrix(scale) * MakeRotateZMatrix(rotate) * MakeTranslateMatrix(translate);
}

// 正射影行列
Matrix4x4 Rendering::MakeOrthographicMatrix(const float& left, const float& top, const float& right, const float& bottom, const float& nearClip, const float& farClip) {
	Matrix4x4 result{
		2.0f / (right - left),0.0f,0.0f,0.0f,
		0.0f,2.0f / (top - bottom),0.0f,0.0f,
		0.0f,0.0f,1.0f / (farClip - nearClip),0.0f,
		(left + right) / (left - right),(top + bottom) / (bottom - top),nearClip / (nearClip - farClip),1.0f
	};

	return result;
}

//透視投影行列
Matrix4x4 Rendering::MakePerspectiveFovMatrix(const float& fovY, const float& aspectRation, const float& nearClip, const float& farClip) {
	Matrix4x4 result{
		1.0f / aspectRation * Math::Cont(fovY / 2.0f),0.0f,0.0f,0.0f,
		0.0f, Math::Cont(fovY / 2.0f),0.0f,0.0f,
		0.0f,0.0f,farClip / (farClip - nearClip),1.0f,
		0.0f,0.0f,-(nearClip * farClip) / (farClip - nearClip),0.0f
	};

	return result;
}

//ビューポートマトリックス
Matrix4x4 Rendering::MakeViewportMatrix(const float& left, const float& top, const float& width, const float& height, const float& minDepth, const float& maxDepth) {
	Matrix4x4 result{
		width / 2.0f,0.0f,0.0f,0.0f,
		0.0f,-height / 2.0f,0.0f,0.0f,
		0.0f,0.0f,maxDepth - minDepth,0.0f,
		(left + width) / 2.0f,(top + height) / 2.0f,minDepth,1.0f,
	};
	return result;
}

//ビルボード行列を作成
Matrix4x4 Rendering::MakeBillboardMatrix(const Matrix4x4& cameraWorldMatrix, const Vector3& rotate) {
	//正面に向けるY軸回転の行列を作成
	Matrix4x4 backToFrontMatrix = Rendering::MakeRotateXYZMatrix(rotate);
	//ビルボード行列を作成
	Matrix4x4 billboardMatrix = backToFrontMatrix * cameraWorldMatrix;
	billboardMatrix.m[3][0] = 0.0f; // X座標を0に設定
	billboardMatrix.m[3][1] = 0.0f; // Y座標を0に設定
	billboardMatrix.m[3][2] = 0.0f; // Z座標を0に設定
	return billboardMatrix;
}

//ビルボード行列を含んだアフィン行列の作成
Matrix4x4 Rendering::MakeBillboardAffineMatrix(const Matrix4x4& cameraWorldMatrix, const Transform& transform) {
	return (MakeScaleMatrix(transform.scale) * MakeBillboardMatrix(cameraWorldMatrix, transform.rotate)) * MakeTranslateMatrix(transform.translate);
}
