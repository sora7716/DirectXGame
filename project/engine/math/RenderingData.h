#pragma once
#include "engine/math/Vector3.h"
#include "engine/math/Vector2.h"
#include "engine/math/Matrix4x4.h"

//Transform情報
typedef struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
	//乗算
	Transform operator*(const Transform transform);
}Transform;

//Transform2D情報
typedef struct Transform2d {
	Vector2 scale;
	float rotate;
	Vector2 translate;
	//乗算
	Transform2d operator*(const Transform2d transform);
}Transform2D;

//TransformationMatrix
typedef struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
}TransformationMatrix;
