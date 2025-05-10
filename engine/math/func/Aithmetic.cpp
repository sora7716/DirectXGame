#include "Aithmetic.h"

const Vector3 operator+(const Vector3& num1,const Vector3& num2){
	Vector3 result{};
	result.x = num1.x + num2.x;
	result.y = num1.y + num2.y;
	result.z = num1.z + num2.z;
	return result;
}

const Vector3& operator+=(Vector3& num1, const Vector3& num2){
	// TODO: return ステートメントをここに挿入します
	num1.x += num2.x;
	num1.y += num2.y;
	num1.z += num2.z;
	return num1;
}

const Vector3 operator-(const Vector3& num1, const Vector3& num2){
	Vector3 result{};
	result.x = num1.x - num2.x;
	result.y = num1.y - num2.y;
	result.z = num1.z - num2.z;
	return result;
}

const Vector3& operator-=(Vector3& num1, const Vector3& num2){
	// TODO: return ステートメントをここに挿入します
	num1.x -= num2.x;
	num1.y -= num2.y;
	num1.z -= num2.z;
	return num1;
}

//スカラー倍
const Vector3 operator*(const Vector3& num, const float& k){
	Vector3 result{};
	result.x = num.x * k;
	result.y = num.y * k;
	result.z = num.z * k;
	return result;
}

const Vector3& operator*=(Vector3& num, const float& k) {
	// TODO: return ステートメントをここに挿入します
	num.x *= k;
	num.y *= k;
	num.z *= k;
	return num;
}

const Vector3 operator*(const Vector3& num1, const Vector3& num2){
	Vector3 result{};
	result.x = num1.x * num2.x;
	result.y = num1.y * num2.y;
	result.z = num1.z * num2.z;
	return result;
}

const Vector3& operator*=(Vector3& num1, const Vector3& num2){
	// TODO: return ステートメントをここに挿入します
	num1.x *= num2.x;
	num1.y *= num2.y;
	num1.z *= num2.z;
	return num1;
}

const Vector3& operator/=(Vector3& num, const float& len){
	// TODO: return ステートメントをここに挿入します
	if (len != 0.0f) {
		num.x /= len;
		num.y /= len;
		num.z /= len;
	}
	return num;
}

const Vector3 operator/(const Vector3& v, const float& num){
	Vector3 result{};
	if (num != 0.0f) {
		result.x = v.x / num;
		result.y = v.y / num;
		result.z = v.z / num;
	}
	return result;
}

const Vector3 operator*(const float& num1, const Vector3& num2){
	Vector3 result{};
	result.x = num1 * num2.x;
	result.y = num1 * num2.y;
	result.z = num1 * num2.z;
	return result;
}

const Vector3& operator+=(Vector3& v, const float& num){
	// TODO: return ステートメントをここに挿入します
	v.x += num;
	v.y += num;
	v.z += num;
	return v;
}

const Vector3 operator-(const Vector3& v){
	return v * -1.0f;
}

const Vector3 operator/(const Vector3& v1, const Vector3& v2){
	Vector3 result;
	result.x = v1.x / v2.x;
	result.y = v1.y / v2.y;
	result.z = v1.z / v2.z;

	return result;
}

const Vector3 operator-(const float& num, const Vector3& v){
	Vector3 result;
	result.x = num - v.x;
	result.y = num - v.y;
	result.z = num - v.z;
	return result;
}

const bool operator<(const Vector3& v1, const Vector3& v2){
	return v1.x < v2.x && v1.y < v2.y && v1.z < v2.z;
}

const Matrix4x4 operator+(const Matrix4x4& num1, const Matrix4x4& num2){
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = num1.m[i][j] + num2.m[i][j];
		}
	}
	return result;
}

const Matrix4x4& operator+=(Matrix4x4& num1, const Matrix4x4& num2){
	// TODO: return ステートメントをここに挿入します
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			num1.m[i][j] += num2.m[i][j];
		}
	}
	return num1;
}

const Matrix4x4 operator-(const Matrix4x4& num1, const Matrix4x4& num2){
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = num1.m[i][j] - num2.m[i][j];
		}
	}
	return result;
}

const Matrix4x4& operator-=(Matrix4x4& num1, const Matrix4x4& num2){
	// TODO: return ステートメントをここに挿入します
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			num1.m[i][j] -= num2.m[i][j];
		}
	}
	return num1;
}

const Matrix4x4 operator*(const Matrix4x4& num1, const Matrix4x4& num2){
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += num1.m[i][k] * num2.m[k][j];
			}
		}
	}
	return result;
}

const Matrix4x4 operator~(const Matrix4x4& num){
	Matrix4x4 result{};
	float determinant = num.m[0][0] * (num.m[1][1] * num.m[2][2] * num.m[3][3] +
		num.m[2][1] * num.m[3][2] * num.m[1][3] +
		num.m[3][1] * num.m[1][2] * num.m[2][3] -
		num.m[3][1] * num.m[2][2] * num.m[1][3] -
		num.m[2][1] * num.m[1][2] * num.m[3][3] -
		num.m[1][1] * num.m[3][2] * num.m[2][3]) -
		num.m[0][1] * (num.m[1][0] * num.m[2][2] * num.m[3][3] +
			num.m[2][0] * num.m[3][2] * num.m[1][3] +
			num.m[3][0] * num.m[1][2] * num.m[2][3] -
			num.m[3][0] * num.m[2][2] * num.m[1][3] -
			num.m[2][0] * num.m[1][2] * num.m[3][3] -
			num.m[1][0] * num.m[3][2] * num.m[2][3]) +
		num.m[0][2] * (num.m[1][0] * num.m[2][1] * num.m[3][3] +
			num.m[2][0] * num.m[3][1] * num.m[1][3] +
			num.m[3][0] * num.m[1][1] * num.m[2][3] -
			num.m[3][0] * num.m[2][1] * num.m[1][3] -
			num.m[2][0] * num.m[1][1] * num.m[3][3] -
			num.m[1][0] * num.m[3][1] * num.m[2][3]) -
		num.m[0][3] * (num.m[1][0] * num.m[2][1] * num.m[3][2] +
			num.m[2][0] * num.m[3][1] * num.m[1][2] +
			num.m[3][0] * num.m[1][1] * num.m[2][2] -
			num.m[3][0] * num.m[2][1] * num.m[1][2] -
			num.m[2][0] * num.m[1][1] * num.m[3][2] -
			num.m[1][0] * num.m[3][1] * num.m[2][2]);



	if (determinant != 0) {
		result.m[0][0] = (num.m[1][1] * num.m[2][2] * num.m[3][3] +
			num.m[2][1] * num.m[3][2] * num.m[1][3] +
			num.m[3][1] * num.m[1][2] * num.m[2][3] -
			num.m[3][1] * num.m[2][2] * num.m[1][3] -
			num.m[2][1] * num.m[1][2] * num.m[3][3] -
			num.m[1][1] * num.m[3][2] * num.m[2][3]) /
			determinant;

		result.m[0][1] = -(num.m[0][1] * num.m[2][2] * num.m[3][3] +
			num.m[2][1] * num.m[3][2] * num.m[0][3] +
			num.m[3][1] * num.m[0][2] * num.m[2][3] -
			num.m[3][1] * num.m[2][2] * num.m[0][3] -
			num.m[2][1] * num.m[0][2] * num.m[3][3] -
			num.m[0][1] * num.m[3][2] * num.m[2][3]) /
			determinant;

		result.m[0][2] = (num.m[0][1] * num.m[1][2] * num.m[3][3] +
			num.m[1][1] * num.m[3][2] * num.m[0][3] +
			num.m[3][1] * num.m[0][2] * num.m[1][3] -
			num.m[3][1] * num.m[1][2] * num.m[0][3] -
			num.m[1][1] * num.m[0][2] * num.m[3][3] -
			num.m[0][1] * num.m[3][2] * num.m[1][3]) /
			determinant;

		result.m[0][3] = -(num.m[0][1] * num.m[1][2] * num.m[2][3] +
			num.m[1][1] * num.m[2][2] * num.m[0][3] +
			num.m[2][1] * num.m[0][2] * num.m[1][3] -
			num.m[2][1] * num.m[1][2] * num.m[0][3] -
			num.m[1][1] * num.m[0][2] * num.m[2][3] -
			num.m[0][1] * num.m[2][2] * num.m[1][3]) /
			determinant;


		result.m[1][0] = -(num.m[1][0] * num.m[2][2] * num.m[3][3] +
			num.m[2][0] * num.m[3][2] * num.m[1][3] +
			num.m[3][0] * num.m[1][2] * num.m[2][3] -
			num.m[3][0] * num.m[2][2] * num.m[1][3] -
			num.m[2][0] * num.m[1][2] * num.m[3][3] -
			num.m[1][0] * num.m[3][2] * num.m[2][3]) /
			determinant;

		result.m[1][1] = (num.m[0][0] * num.m[2][2] * num.m[3][3] +
			num.m[2][0] * num.m[3][2] * num.m[0][3] +
			num.m[3][0] * num.m[0][2] * num.m[2][3] -
			num.m[3][0] * num.m[2][2] * num.m[0][3] -
			num.m[2][0] * num.m[0][2] * num.m[3][3] -
			num.m[0][0] * num.m[3][2] * num.m[2][3]) /
			determinant;

		result.m[1][2] = -(num.m[0][0] * num.m[1][2] * num.m[3][3] +
			num.m[1][0] * num.m[3][2] * num.m[0][3] +
			num.m[3][0] * num.m[0][2] * num.m[1][3] -
			num.m[3][0] * num.m[1][2] * num.m[0][3] -
			num.m[1][0] * num.m[0][2] * num.m[3][3] -
			num.m[0][0] * num.m[3][2] * num.m[1][3]) /
			determinant;

		result.m[1][3] = (num.m[0][0] * num.m[1][2] * num.m[2][3] +
			num.m[1][0] * num.m[2][2] * num.m[0][3] +
			num.m[2][0] * num.m[0][2] * num.m[1][3] -
			num.m[2][0] * num.m[1][2] * num.m[0][3] -
			num.m[1][0] * num.m[0][2] * num.m[2][3] -
			num.m[0][0] * num.m[2][2] * num.m[1][3]) /
			determinant;


		result.m[2][0] = (num.m[1][0] * num.m[2][1] * num.m[3][3] +
			num.m[2][0] * num.m[3][1] * num.m[1][3] +
			num.m[3][0] * num.m[1][1] * num.m[2][3] -
			num.m[3][0] * num.m[2][1] * num.m[1][3] -
			num.m[2][0] * num.m[1][1] * num.m[3][3] -
			num.m[1][0] * num.m[3][1] * num.m[2][3]) /
			determinant;

		result.m[2][1] = -(num.m[0][0] * num.m[2][1] * num.m[3][3] +
			num.m[2][0] * num.m[3][1] * num.m[0][3] +
			num.m[3][0] * num.m[0][1] * num.m[2][3] -
			num.m[3][0] * num.m[2][1] * num.m[0][3] -
			num.m[2][0] * num.m[0][1] * num.m[3][3] -
			num.m[0][0] * num.m[3][1] * num.m[2][3]) /
			determinant;

		result.m[2][2] = (num.m[0][0] * num.m[1][1] * num.m[3][3] +
			num.m[1][0] * num.m[3][1] * num.m[0][3] +
			num.m[3][0] * num.m[0][1] * num.m[1][3] -
			num.m[3][0] * num.m[1][1] * num.m[0][3] -
			num.m[1][0] * num.m[0][1] * num.m[3][3] -
			num.m[0][0] * num.m[3][1] * num.m[1][3]) /
			determinant;

		result.m[2][3] = -(num.m[0][0] * num.m[1][1] * num.m[2][3] +
			num.m[1][0] * num.m[2][1] * num.m[0][3] +
			num.m[2][0] * num.m[0][1] * num.m[1][3] -
			num.m[2][0] * num.m[1][1] * num.m[0][3] -
			num.m[1][0] * num.m[0][1] * num.m[2][3] -
			num.m[0][0] * num.m[2][1] * num.m[1][3]) /
			determinant;

		result.m[3][0] = -(num.m[1][0] * num.m[2][1] * num.m[3][2] +
			num.m[2][0] * num.m[3][1] * num.m[1][2] +
			num.m[3][0] * num.m[1][1] * num.m[2][2] -
			num.m[3][0] * num.m[2][1] * num.m[1][2] -
			num.m[2][0] * num.m[1][1] * num.m[3][2] -
			num.m[1][0] * num.m[3][1] * num.m[2][2]) /
			determinant;

		result.m[3][1] = (num.m[0][0] * num.m[2][1] * num.m[3][2] +
			num.m[2][0] * num.m[3][1] * num.m[0][2] +
			num.m[3][0] * num.m[0][1] * num.m[2][2] -
			num.m[3][0] * num.m[2][1] * num.m[0][2] -
			num.m[2][0] * num.m[0][1] * num.m[3][2] -
			num.m[0][0] * num.m[3][1] * num.m[2][2]) /
			determinant;

		result.m[3][2] = -(num.m[0][0] * num.m[1][1] * num.m[3][2] +
			num.m[1][0] * num.m[3][1] * num.m[0][2] +
			num.m[3][0] * num.m[0][1] * num.m[1][2] -
			num.m[3][0] * num.m[1][1] * num.m[0][2] -
			num.m[1][0] * num.m[0][1] * num.m[3][2] -
			num.m[0][0] * num.m[3][1] * num.m[1][2]) /
			determinant;

		result.m[3][3] = (num.m[0][0] * num.m[1][1] * num.m[2][2] +
			num.m[1][0] * num.m[2][1] * num.m[0][2] +
			num.m[2][0] * num.m[0][1] * num.m[1][2] -
			num.m[2][0] * num.m[1][1] * num.m[0][2] -
			num.m[1][0] * num.m[0][1] * num.m[2][2] -
			num.m[0][0] * num.m[2][1] * num.m[1][2]) /
			determinant;
	}

	return result;

}
