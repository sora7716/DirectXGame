#include "DirectXCommon.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

// DirectX12の初期化
void DirectXCommon::InitializeDirectX12(WinApp* winApp) {
	//ウィンドウズアプリケーションを受け取る
	winApp_ = winApp;
	//IDXIファクトリーの生成
	dxgiFactory_ = MakeIDXGIFactory();
	//使用するアダプタを決定
	useAdapter_ = DecideUseAdapter();
	//D3D12デバイスの生成
	device_ = MakeD3D12Device();
	//警告・エラーがあった場合に実行を止める	
	StopExecution();
	//コマンドキューの生成
	commandQueue_ = MakeCommandQueue();
	//コマンドアローケータの生成
	commandAllocator_ = MakeCommandAllocator();
	//コマンドリストの生成
	commandList_ = MakeCommandList();
	//スワップチェーンの生成
	swapChain_ = MakeSwapChain();
	//ディスクリプタヒープの生成
	rtvDescriptorHeap_ = MakeDescriptorHeap();
	//SwapChainからResourceを引っ張ってくる
	swapChainResources_[0] = BringResourcesFromSwapChain(0);
	swapChainResources_[1] = BringResourcesFromSwapChain(1);
	// RTVの作成
	MakeRTV();
	//Fenceを生成
	fence_ = MakeFence();
}

// IDXIファクトリーの生成
IDXGIFactory7* DirectXCommon::MakeIDXGIFactory() {
	HRESULT result = S_FALSE;
	IDXGIFactory7* factory = nullptr;
	result = CreateDXGIFactory(IID_PPV_ARGS(&factory));
	assert(SUCCEEDED(result));//生成できなかった場合止める
	return factory;
}

//使用するアダプタを決定
IDXGIAdapter4* DirectXCommon::DecideUseAdapter() {
	HRESULT result = S_FALSE;
	IDXGIAdapter4* adapter = nullptr;
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND; i++) {
		//アダプタ情報を受け取る
		DXGI_ADAPTER_DESC3 adapterDesc{};
		result = adapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(result));//取得できない場合止める
		//ソフトウェアアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//採用したアダプタの情報をログに出力。wstringのほうなので注意
			Log::ConsolePrintf(Log::ConvertString(std::format(L"Use Adapter : {}\n", adapterDesc.Description)));
			break;
		}
		adapter = nullptr;//ソフトウェアアダプタの場合は見なかったことにする
	}
	//適切なアダプタが見つからなかった場合止める
	assert(adapter != nullptr);
	return adapter;
}

// D3D12デバイスの生成
ID3D12Device* DirectXCommon::MakeD3D12Device() {
	HRESULT result = S_FALSE;
	ID3D12Device* device = nullptr;
	//機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(useAdapter_, featureLevels[i], IID_PPV_ARGS(&device));
		//指定した機能レベルでデバイスが生成できたか確認
		if (SUCCEEDED(result)) {
			//生成できたのでログを出力を行ってループを抜ける
			Log::ConsolePrintf(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	//デバイスがうまく生成できなかった場合止める
	assert(device != nullptr);
	Log::ConsolePrintf("Complete create D3D12Device!!!\n");//初期化完了のログをだす
	return device;
}

//コマンドキューの生成
ID3D12CommandQueue* DirectXCommon::MakeCommandQueue() {
	HRESULT result = S_FALSE;
	ID3D12CommandQueue* commandQueue = nullptr;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	result = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	//コマンドキューの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));
	return commandQueue;
}

// コマンドアローケータの生成
ID3D12CommandAllocator* DirectXCommon::MakeCommandAllocator() {
	HRESULT result = S_FALSE;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	//コマンドアローケータがうまく生成できなかった場合止める
	assert(SUCCEEDED(result));
	return commandAllocator;
}

// コマンドリストの生成
ID3D12GraphicsCommandList* DirectXCommon::MakeCommandList() {
	HRESULT result = S_FALSE;
	ID3D12GraphicsCommandList* commandList = nullptr;
	result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_, nullptr, IID_PPV_ARGS(&commandList));
	//コマンドリストがうまく生成できなかった場合止める
	assert(SUCCEEDED(result));
	return commandList;
}

//スワップチェーンの生成
IDXGISwapChain4* DirectXCommon::MakeSwapChain() {
	HRESULT result = S_FALSE;
	IDXGISwapChain4* swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = WinApp::kClientWidth;//画面の横幅
	swapChainDesc.Height = WinApp::kClientHeight;//画面の縦幅
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色形式
	swapChainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//描画のターゲットとして利用
	swapChainDesc.BufferCount = 2;//ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//モニタにうつしたら、中身を破棄
	//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	result = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_, winApp_->GetHwnd(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain));
	assert(SUCCEEDED(result));
	return swapChain;
}

// ディスクリプタヒープの生成
ID3D12DescriptorHeap* DirectXCommon::MakeDescriptorHeap() {
	HRESULT result = S_FALSE;
	ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー用
	rtvDescriptorHeapDesc.NumDescriptors = 2;//ダブルバッファ用に2つ。
	result = device_->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap));
	//ディスクリプタヒープがうまく生成できなかった場合止める
	assert(SUCCEEDED(result));
	return rtvDescriptorHeap;
}

// SwapChainからResourceを引っ張ってくる
ID3D12Resource* DirectXCommon::BringResourcesFromSwapChain(UINT num) {
	HRESULT result = S_FALSE;
	ID3D12Resource* swapChainResource = nullptr;
	result = swapChain_->GetBuffer(num, IID_PPV_ARGS(&swapChainResource));
	//うまく取得できなければ起動できない
	assert(SUCCEEDED(result));
	return swapChainResource;
}

//RTVの作成
void DirectXCommon::MakeRTV() {
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;//2dテクスチャとして書き込む
	//ディスクリプタの先頭を取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	//まずは1つ目を作る。1つ目は最初のところに作る。作る場所をこちらで指定してあげる必要がある
	rtvHandles_[0] = rtvStartHandle;
	device_->CreateRenderTargetView(swapChainResources_[0], &rtvDesc, rtvHandles_[0]);
	//2つ目ディスクリプタハンドルを得る(自力で)
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//2つ目を作る
	device_->CreateRenderTargetView(swapChainResources_[1], &rtvDesc, rtvHandles_[1]);
}

//Fenceを作成する
ID3D12Fence* DirectXCommon::MakeFence() {
	HRESULT result = S_FALSE;
	ID3D12Fence* fence = nullptr;
	fenceValue_ = 0;
	result = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(result));

	//FenceのSignalを待つためのイベントを作成
	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent_ != nullptr);
	return fence;
}

// フレームの開始
void DirectXCommon::BeginFrame(){
	/*コマンドを積む*/
	//これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

	//今回のバリアはTransition
	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier_.Transition.pResource = swapChainResources_[backBufferIndex];
	//遷移前(現在)のResourceState
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のResourceState
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier_);

	//描画先のRTVを設定する
	commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex], false, nullptr);
	//指定した色で画面をクリアする
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };//青っぽい色。RGBAの順
	commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex], clearColor, 0, nullptr);

}

// フレームの終了
void DirectXCommon::EndFrame(){
	HRESULT result = S_FALSE;
	//画面に描く処理は全て終わり、画面に移すので、状態を遷移
	//今回はRenderTargetからPresentにする
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier_);

	//コマンドリストの内容を確定させる。全てのコマンドを積んでからCloseすること
	result = commandList_->Close();
	assert(SUCCEEDED(result));

	/*コマンドをキックする*/
	//GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { commandList_ };
	commandQueue_->ExecuteCommandLists(1, commandLists);
	//GPUとOSに画面の交換を行うように通知
	swapChain_->Present(1, 0);
	//Fenceの値を更新
	fenceValue_++;
	//GPUがここまでたどり着いた時に、Fenceの値を指定した値に代入するようにSignalを送る
	commandQueue_->Signal(fence_, fenceValue_);
	if (fence_->GetCompletedValue() < fenceValue_) {
		//指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		//イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);
	}
	//次のフレーム用のコマンドリストを準備
	result = commandAllocator_->Reset();
	assert(SUCCEEDED(result));
	result = commandList_->Reset(commandAllocator_, nullptr);
	assert(SUCCEEDED(result));
}

//デバックレイヤー
void DirectXCommon::DebugLayer() {
#ifdef _DEBUG
	debugController_ = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {
		//デバックレイヤーを有効化する
		debugController_->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugController_->SetEnableGPUBasedValidation(TRUE);
	}
#endif // _DEBUG
}

// 実行を停止する(エラー・警告の場合)
void DirectXCommon::StopExecution() {
#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//やばいエラーの時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		//抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] = {
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
		//解放
		infoQueue->Release();
	}
#endif // _DEBUG
}

//デストラクタ
DirectXCommon::~DirectXCommon() {
	//オブジェクトの開放
	CloseHandle(fenceEvent_);
	fence_->Release();
	rtvDescriptorHeap_->Release();
	swapChainResources_[0]->Release();
	swapChainResources_[1]->Release();
	swapChain_->Release();
	commandList_->Release();
	commandAllocator_->Release();
	commandQueue_->Release();
	device_->Release();
	useAdapter_->Release();
	dxgiFactory_->Release();
#ifdef _DEBUG
	debugController_->Release();
#endif // _DEBUG
	
}
