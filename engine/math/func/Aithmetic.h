#pragma once
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Matrix4x4.h"

/// <summary>
/// 加算
/// </summary>
/// <param name="num1"></param>
/// <param name="num2"></param>
/// <returns></returns>
const Vector3 operator+(const Vector3& num1, const Vector3& num2);

/// <summary>
/// 加算(複合)
/// </summary>
/// <param name="num1"></param>
/// <param name="num2"></param>
/// <returns></returns>
const Vector3& operator+=(Vector3& num1, const Vector3& num2);

/// <summary>
/// 減算
/// </summary>
/// <param name="num1"></param>
/// <param name="num2"></param>
/// <returns></returns>
const Vector3 operator-(const Vector3& num1, const Vector3& num2);

/// <summary>
/// 減算(複合)
/// </summary>
/// <param name="num1"></param>
/// <param name="num2"></param>
/// <returns></returns>
const Vector3& operator-=(Vector3& num1, const Vector3& num2);

/// <summary>
/// スカラー倍
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
const Vector3 operator*(const Vector3& num, const float& k);

/// <summary>
/// スカラー倍(複合)
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
const Vector3& operator*=(Vector3& num, const float& k);

/// <summary>
/// 掛け算
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
const Vector3 operator*(const Vector3& num1, const Vector3& num2);

/// <summary>
/// 掛け算(複合)
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
const Vector3& operator*=(Vector3& num1, const Vector3& num2);

/// <summary>
/// 割り算(float と Vector3の複合)
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
const Vector3& operator/=(Vector3& num, const float& len);

/// <summary>
/// 割り算(float と Vector3)
/// </summary>
/// <param name="num"></param>
/// <param name="len"></param>
/// <returns></returns>
const Vector3 operator/(const Vector3& v, const float& num);

/// <summary>
/// スカラー倍(float*Vector3)
/// </summary>
/// <param name="num1"></param>
/// <param name="num2"></param>
/// <returns></returns>
const Vector3 operator* (const float& num1, const Vector3& num2);

/// <summary>
///加法(Vector3とfloat)
/// </summary>
/// <param name="v"></param>
/// <param name="num"></param>
/// <returns></returns>
const Vector3& operator+=(Vector3& v, const float& num);

/// <summary>
/// マイナスにする
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
const Vector3 operator-(const Vector3& v);

/// <summary>
/// 除算(Vector3とVector3)
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
const Vector3 operator/(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 減算(floatとVector3)
/// </summary>
/// <param name="num"></param>
/// <param name="v"></param>
/// <returns></returns>
const Vector3 operator-(const float& num, const Vector3& v);

/// <summary>
/// v1がv2のほうが小さい
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
const bool operator<(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 加算
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
const Matrix4x4 operator+(const Matrix4x4& num1,const Matrix4x4& num2);

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
