#pragma once
#include "SRVManager.h"
#include "imgui/imgui.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//前方宣言
class DirectXBase;
class SRVManager;
class WinApi;
/// <summary>
/// ImGuiの管理
/// </summary>
class ImGuiManager {
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ImGuiManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ImGuiManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApi">WinApi</param>
	/// <param name="directXBase">DirectXの基盤部分</param>
	/// <param name="srvManager">SRVの管理</param>
	void Initialize(WinApi*winApi,DirectXBase* directXBase, SRVManager* srvManager);

	/// <summary>
	/// 描画開始
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画終了
	/// </summary>
	void PostDraw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
private://メンバ変数
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//SRVの管理
	SRVManager* srvManager_ = nullptr;
	//WinApi
	WinApi* winApi_ = nullptr;
};

