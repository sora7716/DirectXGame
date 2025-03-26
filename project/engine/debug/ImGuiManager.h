#pragma once
#ifdef USE_IMGUI
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"
#endif // USE_IMGUI
//前方宣言
class DirectXBase;
class SRVManager;
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
	/// <param name="directXBase">DirectXの基盤部分</param>
	void Initialize(DirectXBase* directXBase);

	/// <summary>
	/// ImGuiの受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGuiの受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
private://メンバ変数
	//DirectXの基盤部分
	DirectXBase* directXBase_ = nullptr;
	//SRVの管理
	SRVManager* srvManager_ = nullptr;
};

