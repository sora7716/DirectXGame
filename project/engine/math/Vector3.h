#pragma once
/// <summary>
/// 3次元ベクトル
/// </summary>
typedef struct Vector3 final {
	float x, y, z;

	//長さ(ノルム)
	float Length();
	// 正規化
	Vector3 Normalize();

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns>内積</returns>
	float Dot(const Vector3& v);

	/// <summary>
	/// クロス積
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns>クロス積</returns>
	Vector3 Cross(const Vector3& v);

	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="begin">最初のベクトル</param>
	/// <param name="end">最後のベクトル</param>
	/// <param name="frame">フレーム</param>
	/// <returns>現在のベクトル</returns>
	Vector3 Lerp(const Vector3& begin, const Vector3& end, float frame);

	// 円関数を使用した線形補間
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
	//加法(float)
	Vector3 operator+(float n);
	//マイナスにする
	Vector3 operator-()const;
	// vのほうが小さい
	bool operator<(const Vector3& v);

}Vector3;