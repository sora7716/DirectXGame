#include "Input.h"
#include "engine/base/WinApi.h"
#include "engine/camera/Camera.h"
#include "engine/math/func/Rendering.h"
#include "engine/math/func/Math.h"
#include <cassert>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

//インスタンスのゲッター
Input* Input::GetInstance() {
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new Input();
	}
	return instance;
}

//初期化
void Input::Initialize() {
	//ウィンドウズアプリケーションを受け取る
	winApi_ = WinApi::GetInstance();
	//キーボード入力
	KeyboardInitialize();
	//マウス入力
	MouseInitialize();
}

//更新
void Input::Update() {
	//キーボード入力
	KeyboardUpdate();
	//マウス入力
	MouseUpdate();
}

//終了
void Input::Finalize() {
	delete instance;
	instance = nullptr;
	isFinalize = true;
}

// キーの押下をチェック
bool Input::PressKey(BYTE keyNumber) {
	//指定キーを押していればtrueを返す
	if (keys_[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

// キーを押下した瞬間をチェック
bool Input::TriggerKey(BYTE keyNumber) {
	//キーを押下した瞬間ならばtrueを返す
	if (keys_[keyNumber] && !preKeys_[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

// キーを離した瞬間をチェック
bool Input::ReleaseTriggerKey(BYTE keyNumber) {
	//キーを離した瞬間ならばtrueを返す
	if (!keys_[keyNumber] && preKeys_[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

//マウスのボタンの押下をチェック
bool Input::PressMouseButton(Clic mouseClicPos) {
	//マウスの押していればtrueを返す
	if (mouseState_.rgbButtons[mouseClicPos]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

//マウスのボタンの押下した瞬間をチェック
bool Input::TriggerMouseButton(Clic mouseClicPos) {
	//マウスの押していればtrueを返す
	if (mouseState_.rgbButtons[mouseClicPos] && !preMouseState_.rgbButtons[mouseClicPos]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

//マウスのボタンを話した瞬間をチェック
bool Input::ReleaseTriggerMouseButton(Clic mouseClicPos) {
	//マウスの押していればtrueを返す
	if (!mouseState_.rgbButtons[mouseClicPos] && preMouseState_.rgbButtons[mouseClicPos]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

//マウスの移動量のゲッター
const Vector2Int Input::GetMouseMoveAmount() const {
	Vector2Int result = { mouseState_.lX,mouseState_.lY };
	return result;
}

//マウスホイールの回転量のゲッター
const int32_t Input::GetWheelRotate() const{
	int32_t result = mouseState_.lZ;
	return result;
}

//ワールド座標系のマウスの位置のゲッター
const Vector3 Input::GetWorldMousePosition(Camera* camera) const{
	// マウスの座標
	POINT mousePosition;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApi::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	// ビューポートサイズを取得
	float windowWidth = static_cast<float>(WinApi::kClientWidth);
	float windowHeight = static_cast<float>(WinApi::kClientHeight);

	// マウス座標を正規化デバイス座標系(NDC)に変換
	float mouseX = (2.0f * mousePosition.x) / windowWidth - 1.0f;
	float mouseY = 1.0f - (2.0f * mousePosition.y) / windowHeight;
	Vector3 posNDC = { mouseX, mouseY, 1.0f }; // 遠平面(z = 1.0f)でのNDC

	// ビュープロジェクション行列の逆行列を取得
	Matrix4x4 matInverseVP = ~(camera->GetViewMatrix() * camera->GetProjectionMatrix());

	// NDCからワールド座標系への変換（遠平面）
	Vector3 posFar = Rendering::TransformVector(posNDC, matInverseVP);

	// NDCを近平面(z = 0)に調整
	posNDC.z = 0.0f;
	Vector3 posNear = Rendering::TransformVector(posNDC, matInverseVP);

	// マウスレイの方向を計算
	Vector3 mouseDirection = Math::Normalize(posFar - posNear);
	return mouseDirection;
}

//スクリーン座標系のマウスの位置のゲッター
const Vector2Int Input::GetMousePosition() const{
	// マウスの座標
	POINT mousePosition;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApi::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	return Vector2Int(mousePosition.x, mousePosition.y);
}

//キーボード入力の初期化
void Input::KeyboardInitialize() {
	HRESULT result = S_FALSE;
	//DirectInputの初期化
	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(winApi_->GetWndClass().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));
	//入力データ形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(winApi_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

//キーボード入力の更新
void Input::KeyboardUpdate() {
	HRESULT result = S_FALSE;
	// 現在の状態を前回の状態として保存
	memcpy(preKeys_, keys_, sizeof(keys_));
	//キーボード情報の取得開始
	result = keyboard_->Acquire();
	result = keyboard_->GetDeviceState(sizeof(keys_), keys_);
}

//マウス入力の初期化
void Input::MouseInitialize() {
	HRESULT result = S_FALSE;
	//DirectInputの初期化
	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(winApi_->GetWndClass().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result));
	//入力データ形式のセット
	result = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));
	//排他制御レベルのセット
	result = mouse_->SetCooperativeLevel(winApi_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));
}

//マウス入力の更新
void Input::MouseUpdate() {
	HRESULT result = S_FALSE;
	// 現在の状態を前回の状態として保存
	memcpy(&preMouseState_, &mouseState_, sizeof(DIMOUSESTATE));
	//キーボード情報の取得開始
	result = mouse_->Acquire();
	result = mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState_);
}
