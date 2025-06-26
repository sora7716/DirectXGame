#pragma once
typedef struct RGB final {
	float r;
	float g;
	float b;
}RGB;

/// <summary>
/// 4次元ベクトル
/// </summary>
typedef struct Vector4 final {
	float x;
	float y;
	float z;
	float w;

	/// <summary>
	/// RGB値のゲッター
	/// </summary>
	/// <returns>rgb</returns>
	const RGB GetRGB()const;

	/// <summary>
	/// RGBのセッター
	/// </summary>
	/// <param name="rgb"></param>
	void SetRGB(const RGB& rgb);
}Vector4;
