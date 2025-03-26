#include "ImGuiManager.h"
#include "DirectXBase.h"
#include "SRVManager.h"
#include "WinApi.h"
#include<cassert>
//インスタンスのゲッター
ImGuiManager* ImGuiManager::GetInstance() {
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new ImGuiManager();
	}
	return instance;
}

//初期化
void ImGuiManager::Initialize(DirectXBase* directXBase) {
#ifdef USE_IMGUI
	//DirectXの基盤部分を記録する
	directXBase_ = directXBase;
	//SRVの管理を記録する
	srvManager_ = SRVManager::GetInstance();
	IMGUI_CHECKVERSION();
	//ImGuiのコンテキストを生成
	ImGui::CreateContext();
	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(WinApi::GetInstance()->GetHwnd());
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
#endif // USE_IMGUI
}

//ImGuiの受付開始
void ImGuiManager::Begin() {
#ifdef USE_IMGUI
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif // USE_IMGUI
}

//ImGuiの受付終了
void ImGuiManager::End() {
#ifdef USE_IMGUI
	//描画前準備
	ImGui::Render();
#endif // USE_IMGUI
}


//描画
void ImGuiManager::Draw() {
#ifdef USE_IMGUI
	ID3D12GraphicsCommandList* commandList = directXBase_->GetCommandList();
	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvManager_->GetDescriptorHeap() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXBase_->GetCommandList());
#endif // USE_IMGUI
}

//終了
void ImGuiManager::Finalize() {
#ifdef USE_IMGUI
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif // USE_IMGUI
	delete instance;
	instance = nullptr;
	isFinalize = true;
}
