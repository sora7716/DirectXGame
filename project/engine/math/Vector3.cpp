#include "Vector3.h"

//加法
Vector3 Vector3::operator+(const Vector3& v)const {
	return { x + v.x,y + v.y,z + v.z };
}

//減法
Vector3 Vector3::operator-(const Vector3& v)const {
	return { x - v.x,y - v.y,z - v.z };
}

//乗法
Vector3 Vector3::operator*(const Vector3& v) const {
	return { x * v.x,y * v.y,z * v.z };
}

//除法
Vector3 Vector3::operator/(const Vector3& v)const {
	return { x / v.x,y / v.y,z / v.z };
}

//加法(複合)
Vector3& Vector3::operator+=(const Vector3& v) {
	// TODO: return ステートメントをここに挿入します
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

//減法(複合)
Vector3& Vector3::operator-=(const Vector3& v) {
	// TODO: return ステートメントをここに挿入します
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

//乗法(複合)
Vector3& Vector3::operator*=(const Vector3& v) {
	// TODO: return ステートメントをここに挿入します
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

//除法
Vector3& Vector3::operator/=(const Vector3& v) {
	// TODO: return ステートメントをここに挿入します
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

// スカラー倍
Vector3 Vector3::operator*(float n)const {
	return { x * n,y * n,z * n };
}

// スカラー倍(複合)
Vector3& Vector3::operator*=(float n) {
	// TODO: return ステートメントをここに挿入します
	x *= n;
	y *= n;
	z *= n;
	return *this;
}

Vector3 Vector3::operator-(float n) const {
	return { x - n,y - n,z - n };
}

// 除法(float複合)
Vector3& Vector3::operator/=(float n) {
	// TODO: return ステートメントをここに挿入します
	x /= n;
	y /= n;
	z /= n;
	return *this;
}

//除法(float)
Vector3 Vector3::operator/(float n)const {
	return { x / n,y / n,z / n };
}

//加法(float)
Vector3& Vector3::operator+=(float n) {
	// TODO: return ステートメントをここに挿入します
	x -= n;
	y -= n;
	z -= n;
	return*this;
}

//マイナスにする
Vector3 Vector3::operator-()const {
	Vector3 result{
	1.0f - x,
	1.0f - y,
	1.0f - z,
	};
	return result;
}

// vのほうが小さい
bool Vector3::operator<(const Vector3& v) {
	return x < v.x && y < v.y && z < v.z;
}
