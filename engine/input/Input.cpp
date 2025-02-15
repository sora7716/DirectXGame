#include "Input.h"
#include "engine/base/WinApi.h"
#include <cassert>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

//インスタンスのゲッター
Input* Input::GetInstance(){
	static Input instance;
	return &instance;
}

//初期化
void Input::Initialize(std::weak_ptr<WinApi>winApp) {
	HRESULT result = S_FALSE;
	//ウィンドウズアプリケーションを受け取る
	winApi_ = winApp.lock();
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

//更新
void Input::Update() {
	HRESULT result = S_FALSE;
	// 現在の状態を前回の状態として保存
	memcpy(preKey, key, sizeof(key));
	//キーボード情報の取得開始
	result = keyboard_->Acquire();
	result = keyboard_->GetDeviceState(sizeof(key), key);
}

// キーの押下をチェック
bool Input::PressKey(BYTE keyNumber){
	//指定キーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

// キーを押下した瞬間をチェック
bool Input::TriggerKey(BYTE keyNumber){
	//キーを押下した瞬間ならばtrueを返す
	if (key[keyNumber]&&!preKey[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

// キーを離した瞬間をチェック
bool Input::ReleaseTriggerKey(BYTE keyNumber){
	//キーを離した瞬間ならばtrueを返す
	if (!key[keyNumber] && preKey[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}
