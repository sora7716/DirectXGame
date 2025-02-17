#pragma once
#include "engine/math/Vector2.h"
#include "engine/math/Vector3.h"
#include "engine/math/Matrix4x4.h"

#pragma region Vector3
const Vector3 operator*(float n, const Vector3& v);
#pragma endregion Vector3

#pragma region Matrix4x4
/// <summary>
/// 加算
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
const Matrix4x4 operator+(const Matrix4x4& num1, const Matrix4x4& num2);

/// <summary>
///加算(複合) 
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
const Matrix4x4& operator+=(Matrix4x4& num1, const Matrix4x4& num2);

/// <summary>
/// 減算
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
const Matrix4x4 operator-(const Matrix4x4& num1, const Matrix4x4& num2);

/// <summary>
/// 減算(複合)
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
const Matrix4x4& operator-=(Matrix4x4& num1, const Matrix4x4& num2);

/// <summary>
/// 掛け算
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
const Matrix4x4 operator*(const Matrix4x4& num1, const Matrix4x4& num2);

/// <summary>
/// 逆行列
/// </summary>
/// <returns></returns>
const Matrix4x4 operator~(const Matrix4x4& num);
#pragma endregion Matrix4x4