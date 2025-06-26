#include "Vector4.h"

//RGBのゲッター
const RGB Vector4::GetRGB()const {
	return { x,y,z };
}

//RGBのセッター
void Vector4::SetRGB(const RGB& rgb) {
	x = rgb.r;
	y = rgb.g;
	z = rgb.b;
}
