#include "ImGuiManager.h"
#include "DirectXBase.h"
#include "SRVManager.h"
#include "WinApi.h"
//初期化
void ImGuiManager::Initialize(WinApi* winApi, DirectXBase* directXBase, SRVManager* srvManager){
	//DirectXの基盤部分を記録する
	directXBase_ = directXBase;
	//SRVの管理を記録する
	srvManager_ = srvManager;
	//WinApiを記録する
	winApi_ = winApi;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApi_->GetHwnd());
	ImGui_ImplDX12_Init(
		directXBase_->GetDevice(),
		directXBase_->GetSwapChainDesc().BufferCount,
		directXBase_->GetRtvDesc().Format,
		srvManager_->GetDescriptorHeap(),
		srvManager_->GetCPUDescriptorHandle(0),
		srvManager_->GetGPUDescriptorHandle(0));
}

//描画開始
void ImGuiManager::PreDraw(){
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Render();
}

//描画終了
void ImGuiManager::PostDraw(){
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXBase_->GetCommandList());
}

//終了
void ImGuiManager::Finalize(){
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
