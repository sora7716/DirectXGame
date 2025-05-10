#include "Vector2.h"
#include "Vector3.h"

//加法
Vector2 Vector2::operator+(const Vector2& v)const {
	return { x + v.x,y + v.y };
}

//減法
Vector2 Vector2::operator-(const Vector2& v)const {
	return { x - v.x,y - v.y };
}

//乗法
Vector2 Vector2::operator*(const Vector2& v)const {
	return { x * v.x,y * v.y };
}

//除法
Vector2 Vector2::operator/(const Vector2& v)const {
	return { x / v.x,y / v.y };
}

//加法(複合)
Vector2& Vector2::operator+=(const Vector2& v) {
	// TODO: return ステートメントをここに挿入します
	x += v.x;
	y += v.y;
	return *this;
}

//減法(複合)
Vector2& Vector2::operator-=(const Vector2& v) {
	// TODO: return ステートメントをここに挿入します
	x -= v.x;
	y -= v.y;
	return *this;
}

//乗法(複合)
Vector2& Vector2::operator*=(const Vector2& v) {
	// TODO: return ステートメントをここに挿入します
	x *= v.x;
	y *= v.y;
	return *this;
}

//除法(複合)
Vector2& Vector2::operator/=(const Vector2& v) {
	// TODO: return ステートメントをここに挿入します
	x /= v.x;
	y /= v.y;
	return *this;
}

//スカラー倍
Vector2 Vector2::operator*(float n) const {
	return { x * n,y * n };
}

//スカラー倍複合
Vector2& Vector2::operator*=(float n) {
	// TODO: return ステートメントをここに挿入します
	x *= n;
	y *= n;
	return *this;
}

//Vector3を代入
Vector2& Vector2::operator=(const Vector3& v){
	// TODO: return ステートメントをここに挿入します
	x = v.x;
	y = v.y;
	return *this;
}
