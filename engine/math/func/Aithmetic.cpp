#include "Aithmetic.h"

const Vector3 operator*(float n, const Vector3& v) {
	return v - n;
}

const Matrix4x4 operator+(const Matrix4x4& num1, const Matrix4x4& num2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = num1.m[i][j] + num2.m[i][j];
		}
	}
	return result;
}

const Matrix4x4& operator+=(Matrix4x4& num1, const Matrix4x4& num2) {
	// TODO: return ステートメントをここに挿入します
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			num1.m[i][j] += num2.m[i][j];
		}
	}
	return num1;
}

const Matrix4x4 operator-(const Matrix4x4& num1, const Matrix4x4& num2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = num1.m[i][j] - num2.m[i][j];
		}
	}
	return result;
}

const Matrix4x4& operator-=(Matrix4x4& num1, const Matrix4x4& num2) {
	// TODO: return ステートメントをここに挿入します
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			num1.m[i][j] -= num2.m[i][j];
		}
	}
	return num1;
}

const Matrix4x4 operator*(const Matrix4x4& num1, const Matrix4x4& num2) {
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

const Matrix4x4 operator~(const Matrix4x4& num) {
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
