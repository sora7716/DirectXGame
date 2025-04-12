#pragma once
struct Vector3;
/// <summary>
/// 2次元ベクトル
/// </summary>
typedef struct Vector2 final {
	float x, y;
	//加法
	Vector2 operator+(const Vector2& v)const;
	//減法
	Vector2 operator-(const Vector2& v)const;
	//乗法
	Vector2 operator*(const Vector2& v)const;
	//除法
	Vector2 operator/(const Vector2& v)const;
	//加法(複合)
	Vector2& operator+=(const Vector2& v);
	//減法(複合)
	Vector2& operator-=(const Vector2& v);
	//乗法(複合)
	Vector2& operator*=(const Vector2& v);
	//除法(複合)
	Vector2& operator/=(const Vector2& v);
	//スカラー倍
	Vector2 operator*(float n)const;
	//スカラー倍(複合)
	Vector2& operator*=(float n);
	//Vector3を代入
	Vector2& operator=(const Vector3& v);
}Vector2;

/// <summary>
/// 2次元ベクトルの整数型
/// </summary>
typedef struct Vector2Int final {
	int x;
	int y;
}Vector2Int;