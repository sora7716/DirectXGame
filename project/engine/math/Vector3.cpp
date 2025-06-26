#include "Vector3.h"
#include <cmath>
//長さ(ノルム)
float Vector3::Length() {
	float result = std::sqrt(Vector3(x, y, z).Dot(Vector3(x, y, z)));
	return result;
}

//正規化
Vector3 Vector3::Normalize() {
	// TODO: return ステートメントをここに挿入します
	Vector3 result = (Vector3(x, y, z).Length() != 0.0f) ? Vector3(x, y, z) / Vector3(x, y, z).Length() : Vector3(x, y, z);
	return result;
}

//内積
float Vector3::Dot(const Vector3& v) {
	Vector3 tempVector = Vector3(x, y, z) * v;
	float dot = tempVector.x + tempVector.y + tempVector.z;
	return dot;
}

//クロス積
Vector3 Vector3::Cross(const Vector3& v) {
	// TODO: return ステートメントをここに挿入します
	Vector3 result{};
	result.x = y * v.z - z * v.y;
	result.y = z * v.x - x * v.z;
	result.z = x * v.y - y * v.x;
	return result;
}

//線形補間
Vector3 Vector3::Lerp(const Vector3& begin, const Vector3& end, float frame) {
	// TODO: return ステートメントをここに挿入します
	Vector3 result = {};
	result.x = std::lerp(begin.x, end.x, frame);
	result.y = std::lerp(begin.y, end.y, frame);
	result.z = std::lerp(begin.z, end.z, frame);
	return result;
}

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

//加法(float)
Vector3 Vector3::operator+(float n) {
	Vector3 result = {
		x + n,
		y + n,
		z + n,
	};
	return result;
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
