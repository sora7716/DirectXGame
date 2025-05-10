#pragma once
/// <summary>
/// 4x4の行列
/// </summary>
typedef struct Matrix4x4 final {
	float m[4][4];

	//加法
	Matrix4x4 operator+(const Matrix4x4& mat)const;
	//減法
	Matrix4x4 operator-(const Matrix4x4& mat)const;
	//乗法
	Matrix4x4 operator*(const Matrix4x4& mat)const;
	//乗法
	Matrix4x4 operator*(const Matrix4x4* mat)const;
	//加法(複合)
	Matrix4x4& operator+=(const Matrix4x4& mat);
	//減法(複合)
	Matrix4x4& operator-=(const Matrix4x4& mat);
	//逆行列
	Matrix4x4 operator~()const;
}Matrix4x4;