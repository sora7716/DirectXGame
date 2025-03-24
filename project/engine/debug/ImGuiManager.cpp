#include "ImGuiManager.h"
#include "DirectXBase.h"
#include "SRVManager.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"
#include "WinApi.h"
//初期化
void ImGuiManager::Initialize(WinApi* winApi, DirectXBase* directXBase, SRVManager* srvManager) {
	//DirectXの基盤部分を記録する
	directXBase_ = directXBase;
	//SRVの管理を記録する
	srvManager_ = srvManager;
	//WinApiを記録する
	winApi_ = winApi;
	IMGUI_CHECKVERSION();
	//ImGuiのコンテキストを生成
	ImGui::CreateContext();
	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApi_->GetHwnd());
	//srvの確保
	srvManager_->Allocate();
	ImGui_ImplDX12_Init(
		directXBase_->GetDevice(),
		static_cast<int>(directXBase_->GetSwapChainResourceNum()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvManager_->GetDescriptorHeap(),
		srvManager_->GetCPUDescriptorHandle(0),
		srvManager_->GetGPUDescriptorHandle(0));
	//srvの解放
	srvManager_->Free(0);
}

//描画開始
void ImGuiManager::PreDraw() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Render();
}

//描画終了
void ImGuiManager::PostDraw() {
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXBase_->GetCommandList());
}

//終了
void ImGuiManager::Finalize() {
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
