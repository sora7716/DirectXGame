#pragma once
#include "Aithmetic.h"
#include <vector>
#include <cmath>
#include <numbers>
#include <algorithm>

/// <summary>
/// 数学的な計算
/// </summary>
class Math {

public://構造体など
	//バネ
	typedef struct Spring {
		//アンカー｡固定された端の位置
		Vector3 anchor;
		float naturalLength;//自然長
		float stiffness;//剛性。バネ定数。
		float dampingCoefficient;//減衰係数
	}Spring;

	//ボール
	typedef struct Ball {
		Vector3 position;//位置
		Vector3 velocity;//速度
		Vector3 acceleration;//加速度
		float mass;//ボールの重さ
		float radius;//ボールの半径
		uint32_t color;//ボールの色
	}Ball;

	//当たった位置
	typedef struct HitPos {
		bool up;
		bool under;
		bool right;
		bool left;
		bool front;
		bool back;
	}HitPos;

	//ボックス
	typedef struct Box {
		Vector3 position;//位置
		Vector3 velocity;//速度
		Vector3 acceleration;//加速度
		float mass;//ボックスの重さ
		Vector3 size;//ボックスのサイズ
		uint32_t color;//ボックスの色
		float miu = 0.4f;//摩擦係数(摩擦の時)
		float k = 0.6f;//比例定数(空気抵抗の時)
		float e = 0.8f;//反発係数(反発の時)
		HitPos isHit = { false };//オブジェクトに当たっているかどうか
	}Box;

	//振り子
	typedef struct Pendulum {
		Vector3 anchor;//アンカーポイント。固定された端の位置
		float length;//紐の長さ
		float angle;//現在の角度
		float angularVelocity;//角速度ω
		float angularaAcceleration;//角加速度
	}Pendulum;

	//円錐に回る振り子
	typedef struct ConicalPendulum {
		Vector3 anchor;//アンカーポイント
		float length;//紐の長さ
		float halfApexAngle;//円錐の頂点の半分の角度
		float angle;//現在の角度
		float angularVelocity;//角速度ω
	}ConicalPendulum;
public:

	/// <summary>
	/// 転置行列
	/// </summary>
	/// <param name="m">マトリック</param>
	/// <returns>matrixの転置行列</returns>
	static Matrix4x4 Transpose(Matrix4x4 m);

	/// <summary>
	/// 単位行列
	/// </summary>
	/// <returns>単位行列</returns>
	static Matrix4x4 MakeIdentity4x4();

	/// <summary>
	/// クロス積
	/// </summary>
	/// <param name="v1">vector1</param>
	/// <param name="v2">vector2</param>
	/// <returns></returns>
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="v1">vector1</param>
	/// <param name="v2">vector2</param>
	/// <returns></returns>
	static float Dot(const Vector3& v1, const Vector3& v2);

	/// <summary>
	///	ノルム(Vector3)
	/// </summary>
	/// <param name="v">vector</param>
	/// <returns></returns>
	static float Length(const Vector3& v);

	/// <summary>
	/// ノルム(float)
	/// </summary>
	/// <param name="num">数字</param>
	/// <returns></returns>
	static float Length(float num);

	/// <summary>
	/// 正規化vector
	/// </summary>
	/// <param name="v">vector</param>
	/// <returns></returns>
	static Vector3 Normalize(const Vector3& v);

	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="num">数字</param>
	/// <returns></returns>
	static float Normalize(const float& num);

	/// <summary>
	/// 正射影ベクトル
	/// </summary>
	/// <param name="v1">ベクトル</param>
	/// <param name="v2">ベクトル</param>
	/// <returns>正射影</returns>
	static Vector3 Project(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// トランスフォームノーマル
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <param name="m">マトリックス</param>
	/// <returns></returns>
	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <param name="t">媒介変数</param>
	/// <returns>線形補間</returns>
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <param name="t">媒介変数</param>
	/// <returns> 球面線形補間</returns>
	static Vector3 SLerp(const Vector3& v1, const Vector3& v2, float t);

	/// <summary>
	/// CatmullRom補間
	/// </summary>
	/// <param name="p0">点0の座標</param>
	/// <param name="p1">点1の座標</param>
	/// <param name="p2">点2の座標</param>
	/// <param name="p3">点3の座標</param>
	/// <param name= "t">点 1を0.0f点2を1.0fとした割合指定</param>
	/// <returns>CatmullRom補間</returns>
	static Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

	/// <summary>
	/// CatmullRomスプライン曲線上の座標を得る
	/// </summary>
	/// <param name="points">制御点の集合</param>
	/// <param name="t">スプラインの全区間の中での割合指定[0,1]</param>
	/// <returns>CatmullRomスプライン曲線上</returns>
	static Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);

	/// <summary>
	/// 三次元のベジエ曲線
	/// </summary>
	/// <param name="points">制御点</param>
	/// <param name="t">フレーム</param>
	/// <returns>ベジエ曲線</returns>
	static Vector3 Bezier(const Vector3* points, float t);

	/// <summary>
	/// 三次元ベジエ曲線(球面線形補間ver)
	/// </summary>
	/// <param name="points">制御点</param>
	/// <param name="t">フレーム</param>
	/// <returns>ベジエ曲線</returns>
	static Vector3 BezierS(const Vector3* points, float t);

	/// <summary>
	/// フックの法則
	/// </summary>
	/// <param name="spring">バネ</param>
	/// <param name="ball">ボール</param>
	/// <param name="isGravityOn">重力をつけるかどうか</param>
	static void Hooklaw(const Spring& spring, Ball& ball, bool isGravityOn = false);

	/// <summary>
	/// 円運動XY
	/// </summary>
	/// <param name="centerPos">中心</param>
	/// <param name="ballPos">現在のボールの位置</param>
	static void CircularMoveXY(const Vector3& centerPos, Vector3& ballPos, const Vector2& radius);

	/// <summary>
	/// 円運動XZ
	/// </summary>
	/// <param name="centerPos">中心</param>
	/// <param name="ballPos">現在のボールの位置</param>
	static void CircularMoveXZ(const Vector3& centerPos, Vector3& ballPos, const Vector2& radius);

	/// <summary>
	/// 円運動ZY
	/// </summary>
	/// <param name="centerPos">中心</param>
	/// <param name="ballPos">現在のボールの位置</param>
	static void CircularMoveZY(const Vector3& centerPos, Vector3& ballPos, const Vector2& radius);

	/// <summary>
	/// 振り子の作成
	/// </summary>
	/// <param name="pendulum">振り子ワイヤー</param>
	/// <param name="ballPos">振り子の先につくもの</param>
	static void MakePendulum(Pendulum& pendulum, Vector3& ballPos);

	/// <summary>
	/// 円錐状に動く振り子を作成
	/// </summary>
	/// <param name="conicalPendulum">円錐状に動く振り子のワイヤー</param>
	/// <param name="ballPos">振り子の先につくもの</param>
	static void MakeConicalPendulum(ConicalPendulum& conicalPendulum, Vector3& ballPos);

	/// <summary>
	/// 反射ベクトル
	/// </summary>
	/// <param name="input">入射ベクトル</param>
	/// <param name="normal">面の法線</param>
	/// <returns>反射ベクトル</returns>
	static Vector3 ReflectVector(const Vector3& input, const Vector3& normal);

	/// <summary>
	/// 反発
	/// </summary>
	/// <param name="objectVelocity">オブジェクトの速度</param>
	/// <param name="normal">衝突対象の法線ベクトル</param>
	/// <param name="e">反発係数</param>
	static Vector3 Reflection(const Vector3& objectVelocity, const Vector3 normal, float e);

	/// <summary>
	/// 重力のオンオフ
	/// </summary>
	/// <param name="velocity">速度</param>
	/// <returns>ON/OFF</returns>
	static bool GravityOnOff(Vector3& velocity, bool isFall);

	/// <summary>
	/// 空気抵抗
	/// </summary>
	/// <param name="velocity">速度</param>
	/// <param name="mass">質量</param>
	/// <param name="k">比例定数</param>
	/// <returns>抵抗(加速度)</returns>
	static Vector3 AirResistance(const Vector3& velocity, float mass, float k);

	/// <summary>
	/// 摩擦
	/// </summary>
	/// <param name="velocity">速度</param>
	/// <param name="mass">質量</param>
	/// <param name="miu">摩擦係数</param>
	/// <returns>抵抗(加速度)</returns>
	static Vector3 Friction(Vector3& velocity, float mass, float miu);

	/// <summary>
	/// リサージュ曲線
	/// </summary>
	/// <param name="theta">θ</param>
	/// <param name="center">中心点</param>
	/// <param name="scalar">スカラー</param>
	/// <returns>閉曲線</returns>
	static Vector3 LissajousCurve(const Vector3& theta, const Vector3& center, const Vector3& scalar = { 1.0f,1.0f,1.0f });

	/// <summary>
	/// 逆正接関数のcotangent
	/// </summary>
	/// <param name="theta">θ</param>
	/// <returns>逆正接関数</returns>
	static float Cont(float theta);
public://定数
	//デルタタイム
	static inline const float kDeltaTime = 1.0f / 60.0f;
	//重力加速度
	static inline const float kGravity = -9.8f;
	//円周率
	static inline const float kPi = std::numbers::pi_v<float>;
	//ラジアン変換用定数
	static inline const float kRad = std::numbers::pi_v<float> / 180.0f;

private://メンバ変数
	static inline bool defaultFall_ = true;
};