#pragma once
/// <summary>
/// 3次元ベクトル
/// </summary>
typedef struct Vector3 final {
	float x;
	float y;
	float z;

	//加法
	Vector3 operator+(const Vector3& v)const;
	//減法
	Vector3 operator-(const Vector3& v)const;
	//乗法
	Vector3 operator*(const Vector3& v)const;
	//除法
	Vector3 operator/(const Vector3& v)const;
	//加法(複合)
	Vector3& operator+=(const Vector3& v);
	//減法(複合)
	Vector3& operator-=(const Vector3& v);
	//乗法(複合)
	Vector3& operator*=(const Vector3& v);
	//除法(複合)
	Vector3& operator/=(const Vector3& v);
	// スカラー倍
	Vector3 operator*(float n)const;
	// スカラー倍(複合)
	Vector3& operator*=(float n);
	//減法(float)
	Vector3 operator-(float n)const;
	// 除法(float複合)
	Vector3& operator/=(float n);
	//除法(float)
	Vector3 operator/(float n)const;
	//加法(float)
	Vector3& operator+=(float n);
	//マイナスにする
	Vector3 operator-()const;
	// vのほうが小さい
	bool operator<(const Vector3& v);

}Vector3;