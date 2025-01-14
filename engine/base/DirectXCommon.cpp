#include "DirectXCommon.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")


//インスタンスのゲッター
DirectXCommon* DirectXCommon::GetInstance() {
	static DirectXCommon instance;
	return &instance;
}

// DirectX12の初期化
void DirectXCommon::InitializeDirectX12(){
	//IDXIファクトリーの生成
	MakeIDXGIFactory();
	//使用するアダプタを決定
	DecideUseAdapter();
	//D3D12デバイスの生成
	MakeD3D12Device();
}

// IDXIファクトリーの生成
void DirectXCommon::MakeIDXGIFactory() {
	hr_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(hr_));//生成できなかった場合止める
}

//使用するアダプタを決定
void DirectXCommon::DecideUseAdapter() {
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) != DXGI_ERROR_NOT_FOUND; i++) {
		//アダプタ情報を受け取る
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr_ = useAdapter_->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr_));//取得できない場合止める
		//ソフトウェアアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//採用したアダプタの情報をログに出力。wstringのほうなので注意
			Log::ConsolePrintf(Log::ConvertString(std::format(L"Use Adapter : {}\n", adapterDesc.Description)));
			break;
		}
		useAdapter_ = nullptr;//ソフトウェアアダプタの場合は見なかったことにする
	}
	//適切なアダプタが見つからなかった場合止める
	assert(useAdapter_ != nullptr);
}

// D3D12デバイスの生成
void DirectXCommon::MakeD3D12Device() {
	//機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		//採用したアダプターでデバイスを生成
		hr_ = D3D12CreateDevice(useAdapter_.Get() , featureLevels[i], IID_PPV_ARGS(&device_));
		//指定した機能レベルでデバイスが生成できたか確認
		if (SUCCEEDED(hr_)) {
			//生成できたのでログを出力を行ってループを抜ける
			Log::ConsolePrintf(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	//デバイスがうまく生成できなかった場合止める
	assert(device_ != nullptr);
	Log::ConsolePrintf("Complete create D3D12Device!!!\n");//初期化完了のログをだす
}

//コマンドキューの生成
void DirectXCommon::MakeCommandQueue(){
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	hr_ = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	//コマンドキューの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr_));
}

// コマンドアローケータの生成
void DirectXCommon::MakeCommandAllocator(){
	hr_ = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	//コマンドアローケータがうまく生成できなかった場合止める
	assert(SUCCEEDED(hr_));
}

// コマンドリストの生成
void DirectXCommon::MakeCommandList(){
	hr_ = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	//コマンドリストがうまく生成できなかった場合止める
	assert(SUCCEEDED(hr_));
}
