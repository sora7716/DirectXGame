#pragma once
#include "engine/math/ResourceData.h"
#include "engine/3d/Model.h"
#include <vector>
#include <string>
#include <wrl.h>
#include <d3d12.h>

//前方宣言
class Camera;

//正規化デバイスするときに使用する
typedef struct PlanePoint {
	float left;
	float top;
	float right;
	float bottom;
}PlanePoint;
/// <summary>
/// 平面のオブジェクト
/// </summary>
class PlaneObject {
private://エイリアステンプレート
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlaneObject() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlaneObject() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// モデルのセッター
	/// </summary>
	/// <param name="name">モデルの名前</param>
	void SetModel(const std::string& name);

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera);

	/// <summary>
	/// スケールのセッター
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(const Vector2& scale);

	/// <summary>
	/// 回転のセッター
	/// </summary>
	/// <param name="rotate">回転</param>
	void SetRotate(float rotate);

	/// <summary>
	/// 平行移動のセッター
	/// </summary>
	/// <param name="translate">平行移動</param>
	void SetTranslate(const Vector2& translate);

	/// <summary>
	/// トランスフォームのセッター
	/// </summary>
	/// <param name="transform">トランスフォーム2D</param>
	void SetTransform(const Transform2d& transform2d);

	/// <summary>
	/// uvスケールのセッター
	/// </summary>
	/// <param name="uvScale">スケール</param>
	void SetUVScale(const Vector2& uvScale);

	/// <summary>
	/// uv回転のセッター
	/// </summary>
	/// <param name="uvRotate">回転</param>
	void SetUVRotate(const float uvRotate);

	/// <summary>
	/// uv平行移動のセッター
	/// </summary>
	/// <param name="uvTranslate">平行移動</param>
	void SetUVTranslate(const Vector2& uvTranslate);

	/// <summary>
	/// 色のセッター
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color);

	/// <summary>
	/// 親のセッター
	/// </summary>
	/// <param name="parent">親</param>
	void SetParent(const Matrix4x4* parent);

	/// <summary>
	/// テクスチャの変更(.pngはいらない)
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void SetTexture(const std::string& filePath);

	/// <summary>
	/// スケールのゲッター
	/// </summary>
	/// <returns>スケール</returns>
	const Vector2& GetScale()const;

	/// <summary>
	/// 回転のゲッター
	/// </summary>
	/// <returns>回転</returns>
	const float GetRotate()const;

	/// <summary>
	/// 平行移動のゲッター
	/// </summary>
	/// <returns>平行移動</returns>
	const Vector2& GetTranslate()const;

	/// <summary>
	/// uvスケールのゲッター
	/// </summary>
	/// <returns>uvスケール</returns>
	const Vector2& GetUVScale()const;

	/// <summary>
	/// uv回転のゲッター
	/// </summary>
	/// <returns>uv回転</returns>
	const float GetUVRotate()const;

	/// <summary>
	/// uv平行移動のゲッター
	/// </summary>
	/// <returns>uv平行移動</returns>
	const Vector2& GetUVTranslate()const;

	/// <summary>
	/// 色のゲッター
	/// </summary>
	/// <returns>色</returns>
	const Vector4& GetColor()const;

	/// <summary>
	/// ワールド行列のゲッター
	/// </summary>
	/// <returns>ワールド行列</returns>
	const Matrix4x4& GetWorldMatrix()const;
private://メンバ関数
	/// <summary>
	/// 座標変換行列リソースの生成
	/// </summary>
	void CreateTransformationMatrixResource();

	/// <summary>
	/// 光源の生成
	/// </summary>
	void CreateDirectionLight();
private://メンバ変数
	//ローカル座標
	Transform2d transform2d_ = {};
	//UV座標
	Transform2d uvTransform_ = {
		.scale = { 1.0f,1.0f },
		.rotate = 0.0f,
		.translate = {0.0f,0.0f}
	};
	//カメラ
	Camera* camera_ = nullptr;
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//3Dモデル
	Model* model_ = nullptr;
	//バッファリソース
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;//座標変換行列
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;//光源
	//バッファリソース内のデータを指すポインタ
	TransformationMatrix* wvpData_ = nullptr;//座標変換行列
	DirectionalLight* directionalLightData_ = nullptr;//光源
	//親
	const Matrix4x4* parent_ = nullptr;
	Matrix4x4 saveWorldMatrix_ = {};
};


