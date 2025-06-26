#define NOMINMAX
#include "Math.h"
#include <cassert>
using namespace std;

//転置行列
Matrix4x4 Math::Transpose(Matrix4x4 m) {
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m.m[j][i];
		}
	}

	return result;
}

//単位行列
Matrix4x4 Math::MakeIdentity4x4() {
	Matrix4x4 result{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};
	return result;
}

//クロス積
Vector3 Math::Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

//内積
float Math::Dot(const Vector3& v1, const Vector3& v2) {
	Vector3 result = {};
	result = v1 * v2;
	float dot = result.x + result.y + result.z;
	return dot;
}

//ノルム
float Math::Length(const Vector3& v) {
	Vector3 result = {};
	result = v * v;
	float length = sqrtf(result.x + result.y + result.z);
	return length;
}

//ノルム
float Math::Length(float num) {
	return sqrtf((float)pow(num, 2));
}


//単位ベクトル
Vector3 Math::Normalize(const Vector3& v) {
	float len = Length(v);
	Vector3 result{};
	result = v / len;
	return result;
}

//正規化
float Math::Normalize(const float& num) {
	float len = powf(num, 2);
	if (len != 0.0f) {
		float result = num / len;
		return result;
	}
	return num;
}

//正射影ベクトル
Vector3 Math::Project(const Vector3& v1, const Vector3& v2) {
	float length = Math::Dot(v2, v2); // v2の長さの2乗

	// 0除算を避けるために長さがゼロでないか確認
	if (length == 0.0f) {
		return Vector3(0, 0, 0); // 長さが0のベクトルに射影すると結果は無効
	}

	float dot1 = Math::Dot(v1, v2);
	return (dot1 / length) * v2;
}

// トランスフォームノーマル
Vector3 Math::TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};
	return result;
}

// 線形補間
Vector3 Math::Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;
	result.x = v1.x + t * (v2.x - v1.x);
	result.y = v1.y + t * (v2.y - v1.y);
	result.z = v1.z + t * (v2.z - v1.z);

	return result;
}

// 球面線形補間
Vector3 Math::SLerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 nv1 = Normalize(v1); // v1 の正規化ベクトル
	Vector3 nv2 = Normalize(v2); // v2 の正規化ベクトル
	float dot = Dot(nv1, nv2);   // 正規化されたベクトル同士の内積

	// 誤差により1.0fを超えるのを防ぐ
	dot = std::clamp(dot, -1.0f, 1.0f);

	// アークコサインでθの角度を求める
	float theta = std::acos(dot);
	float sinTheta = std::sin(theta);

	Vector3 normalizeVector;
	// ゼロ除算を防ぐ
	if (sinTheta < 1.0e-5) {
		normalizeVector = nv1;
	} else {
		// 球面線形補間したベクトル(単位ベクトル)
		float sinThetaFrom = std::sin((1 - t) * theta);
		float sinThetaTo = std::sin(t * theta);
		normalizeVector = (sinThetaFrom * nv1 + sinThetaTo * nv2) / sinTheta;
	}

	// ベクトルの長さはv1とv2の長さを線形補間
	float length1 = Length(v1);
	float length2 = Length(v2);
	// Lerpで補間ベクトルの長さを求める
	float length = std::lerp(length1, length2, t);

	return normalizeVector * length;
}

// CatmullRom補間
Vector3 Math::CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
	const float s = 0.5f; // 1/2のこと

	float t2 = t * t;  // tの2乗
	float t3 = t2 * t; // tの3乗

	Vector3 e3 = p0 * -1 + 3 * p1 - 3 * p2 + p3;
	Vector3 e2 = 2 * p0 - 5 * p1 + 4 * p2 - p3;
	Vector3 e1 = p0 * -1 + p2;
	Vector3 e0 = 2 * p1;

	return s * (e3 * t3 + e2 * t2 + e1 * t + e0);
}

// CatmullRomスプライン曲線上の座標を得る
Vector3 Math::CatmullRomPosition(const std::vector<Vector3>& points, float t) {
	assert(points.size() >= 4 && "制御点は4点以上必要です");
	// 区間数は制御点の数-1
	size_t division = points.size() - 1;
	// 1区間の長さ(全体を1.0とした割合)
	float areaWidth = 1.0f / division;

	// 区間内の始点を0.0f、終点1.0fとしたときの現在位置
	float t_2 = std::fmod(t, areaWidth) * division;
	// 下限(0.0f)とz上限(1.0f)の範囲を収める
	t_2 = std::clamp(t_2, 0.0f, 1.0f);
	// 区間番号
	size_t index = static_cast<size_t>(t / areaWidth);
	// 区間番号が上限を超えないように収める
	index = std::min(index, division - 1);

	// 4点分のインデックス
	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	// 最初の区間のp0はp1を重複使用する
	if (index == 0) {
		index0 = index1;
	}

	// 最後の区間のp3はp2を重複使用する
	if (index3 >= points.size()) {
		index3 = index2;
	}

	// 4点の座標
	const Vector3& p0 = points[index0];
	const Vector3& p1 = points[index1];
	const Vector3& p2 = points[index2];
	const Vector3& p3 = points[index3];

	// 4点を指定してCatmull-Rom補間
	return CatmullRomInterpolation(p0, p1, p2, p3, t_2);
}

// 三次元のベジエ曲線
Vector3 Math::Bezier(const Vector3* points, float t) {
	//制御点を分ける
	Vector3 p0 = points[0];//始点
	Vector3 p1 = points[1];//中点
	Vector3 p2 = points[2];//終点

	Vector3 p0p1 = Lerp(p0, p1, t);  // p0とp1の間を補間
	Vector3 p1p2 = Lerp(p1, p2, t);  // p1とp2の間を補間
	Vector3 p = Lerp(p0p1, p1p2, t); // 上記2つの補間結果をさらに補間
	return p;
}

// 三次元ベジエ曲線(球面線形補間ver)
Vector3 Math::BezierS(const Vector3* points, float t) {
	//制御点を分ける
	Vector3 p0 = points[0];//始点
	Vector3 p1 = points[1];//中点
	Vector3 p2 = points[2];//終点

	Vector3 p0p1 = SLerp(p0, p1, t);  // p0とp1の間を補間
	Vector3 p1p2 = SLerp(p1, p2, t);  // p1とp2の間を補間
	Vector3 p = SLerp(p0p1, p1p2, t); // 上記2つの補間結果をさらに補間
	return p;
}

//フックの法則
void Math::Hooklaw(const Spring& spring, Ball& ball, bool isGravityOn) {
	//重力加速度
	if (isGravityOn) {
		ball.acceleration += kGravity;
	}
	Vector3 diff = ball.position - spring.anchor;
	float length = Length(diff);
	if (length != 0.0f) {
		Vector3 direction = Normalize(diff);
		Vector3 restPosition = spring.anchor + direction * spring.naturalLength;//止まる位置
		Vector3 displacement = length * (ball.position - restPosition);//変位ベクトル
		Vector3 restoringForce = -spring.stiffness * displacement;//復元力
		Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;//減衰抵抗を計算する
		Vector3 force = restoringForce + dampingForce;//力の向き(減衰抵抗も加味して、物体にかかる力を決定する)
		ball.acceleration = force / std::abs(ball.mass) + (ball.acceleration / kDeltaTime);//加速度に力/質量を代入
	}
	//加速度も速度どちらも秒を基準とした値である
	//それが、1/60秒間(deltaTime)適用されたと考える
	ball.velocity += ball.acceleration * kDeltaTime;
	ball.position += ball.velocity * kDeltaTime;
}

// 円運動XY
void Math::CircularMoveXY(const Vector3& centerPos, Vector3& ballPos, const Vector2& radius) {
	float angularVelocity = kPi;//角速度
	static float angle = 0.0f;//角度
	angle += angularVelocity * kDeltaTime;//現在の角度の計算
	//円運動させる
	ballPos.x = centerPos.x + cos(angle) * radius.x;
	ballPos.y = centerPos.y + sin(angle) * radius.y;
	ballPos.z = centerPos.z;
}

// 円運動XZ
void Math::CircularMoveXZ(const Vector3& centerPos, Vector3& ballPos, const Vector2& radius) {
	float angularVelocity = kPi;//角速度
	static float angle = 0.0f;//角度
	angle += angularVelocity * kDeltaTime;//現在の角度の計算
	//円運動させる
	ballPos.x = centerPos.x + cos(angle) * radius.x;
	ballPos.y = centerPos.y;
	ballPos.z = centerPos.z + sin(angle) * radius.y;
}

// 円運動ZY
void Math::CircularMoveZY(const Vector3& centerPos, Vector3& ballPos, const Vector2& radius) {
	float angularVelocity = kPi;//角速度
	static float angle = 0.0f;//角度
	angle += angularVelocity * kDeltaTime;//現在の角度の計算
	//円運動させる
	ballPos.x = centerPos.x;
	ballPos.y = centerPos.y + sin(angle) * radius.y;
	ballPos.z = centerPos.z + cos(angle) * radius.x;
}

//振り子の作成
void Math::MakePendulum(Pendulum& pendulum, Vector3& ballPos) {
	pendulum.angularaAcceleration = -(abs(Math::kGravity) / pendulum.length) * sin(pendulum.angle);
	pendulum.angularVelocity += pendulum.angularaAcceleration * kDeltaTime;
	pendulum.angle += pendulum.angularVelocity * kDeltaTime;
	//振り子の先端
	ballPos.x = pendulum.anchor.x + sin(pendulum.angle) * pendulum.length;
	ballPos.y = pendulum.anchor.y - cos(pendulum.angle) * pendulum.length;
	ballPos.z = pendulum.anchor.z;
}

// 円錐状に動く振り子を作成
void Math::MakeConicalPendulum(ConicalPendulum& conicalPendulum, Vector3& ballPos) {
	//角度を計算
	conicalPendulum.angularVelocity = sqrt(9.8f / (conicalPendulum.length * cos(conicalPendulum.halfApexAngle)));
	conicalPendulum.angle += conicalPendulum.angularVelocity * kDeltaTime;

	//求めた角度からボブの位置を算出
	float radius = sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
	float height = cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;
	ballPos.x = conicalPendulum.anchor.x + cos(conicalPendulum.angle) * radius;
	ballPos.y = conicalPendulum.anchor.y - height;
	ballPos.z = conicalPendulum.anchor.z - sin(conicalPendulum.angle) * radius;
}

//反射ベクトル
Vector3 Math::ReflectVector(const Vector3& input, const Vector3& normal) {
	return input - 2 * Math::Dot(input, normal) * normal;
}

//反発
Vector3 Math::Reflection(const Vector3& objectVelocity, const Vector3 normal, float e) {
	Vector3 reflected = ReflectVector(objectVelocity, normal);
	Vector3 projectToNormal = Project(reflected, normal);
	Vector3 movingDirection = reflected - projectToNormal;
	Vector3 refrectionVelocity = projectToNormal * e + movingDirection;
	return refrectionVelocity;
}

//重力のオンオフ
bool Math::GravityOnOff(Vector3& velocity, bool isFall) {
	if (velocity.y < 0.1f) {
		isFall = false;
	}
	if (!isFall) {
		velocity = { velocity.x,0.0f,velocity.z };
	}
	return isFall;
}

//空気抵抗
Vector3 Math::AirResistance(const Vector3& velocity, float mass, float k) {
	Vector3 acceleration{};
	// 速度の大きさ（ノルム）を計算
	float speed = Math::Length(velocity);

	// 速度がゼロでない場合のみ空気抵抗を計算
	if (speed > 0.0f) {
		// 空気抵抗の力を計算 (速度の二乗に比例)
		Vector3 airResistance = -k * pow(speed, 2.0f) * Math::Normalize(velocity);

		// 空気抵抗による加速度を計算
		Vector3 airResistanceAcceleration = airResistance / mass;

		// 総合加速度に空気抵抗と重力を加算
		acceleration = airResistanceAcceleration + kGravity;
	}
	return acceleration;
}

//摩擦
Vector3 Math::Friction(Vector3& velocity, float mass, float miu) {
	Vector3 acceleration{};
	// 動いていたら
	if (fabs(velocity.x) > 0.01f || fabs(velocity.y) > 0.01f || fabs(velocity.z) > 0.01f) {
		// 摩擦力の大きさを計算
		float magnitude = miu * Math::Length(-mass * kGravity);

		// 摩擦力の向き（速度の逆方向）
		Vector3 direction = Normalize(-velocity);

		// 摩擦力を計算
		Vector3 frictionalForce = magnitude * direction;

		// 加速度に摩擦力を反映（力を質量で割る）
		acceleration += frictionalForce / mass;

		// 摩擦力によって速度がゼロになる場合、速度と加速度を停止
		if (fabs(frictionalForce.x * kDeltaTime) > fabs(velocity.x) ||
			fabs(frictionalForce.y * kDeltaTime) > fabs(velocity.y) ||
			fabs(frictionalForce.z * kDeltaTime) > fabs(velocity.z)) {
			acceleration = -velocity * 60.0f;
		}
	}
	return acceleration;
}

// リサージュ曲線
Vector3 Math::LissajousCurve(const Vector3& theta, const Vector3& center, const Vector3& scalar) {
	Vector3 result{};
	result.x = scalar.x * sin(theta.x) + center.x;
	result.y = scalar.y * sin(theta.y) + center.y;
	result.z = scalar.z * sin(theta.z) + center.z;
	return result;
}

//逆正接関数のcotangent
float Math::Cont(float theta) {
	return (1.0f / tanf(theta));
}
