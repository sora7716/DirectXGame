#include "DirectXBase.h"
#include "engine/base/FixFPS.h"
#include <cassert>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")
using namespace Microsoft::WRL;
const uint32_t DirectXBase::kMaxSRVCount = 512;

// DirectX12の初期化
void DirectXBase::Initialize() {
	//FPS固定初期化
	FixFPS::GetInstance()->Initialize();
	//ウィンドウズアプリケーションを受け取る
	winApi_ = WinApi::GetInstance();
	//デバイスの初期化
	InitializeDevice();
	//コマンド関連の初期化
	InitializeCommand();
	//スワップチェーンの生成
	CreateSwapChain();
	//深度バッファの生成
	CreateDepthBuffer();
	//各種デスクリプタヒープの生成
	CreateDescriptorHeap();
	//レンダーターゲットビューの初期化
	InitializeRTV();
	//深度ステンシルビューの初期化
	InitializeDepthStencil();
	//フェンスの初期化
	InitializeFence();
	//ビューポート矩形の初期化
	InitializeViewport();
	//シザリング矩形の初期化
	InitializeScissorRect();
	//DXCコンパイラの生成
	CreateDXCCompiler();
	//ImGuiの初期化
	InitializeImGui();
}

//デバイスの初期化
void DirectXBase::InitializeDevice() {
	//デバックレイヤーをオン
	DebugLayer();
	//IDXIファクトリーの生成
	dxgiFactory_ = MakeIDXGIFactory();
	//アダプターの列挙
	useAdapter_ = DecideUseAdapter();
	//デバイス生成
	device_ = MakeD3D12Device();
	//エラー時にブレークを発生させる設定
	StopExecution();
}

//コマンド関連の初期化
void DirectXBase::InitializeCommand() {
	//コマンドアローケータの生成
	commandAllocator_ = MakeCommandAllocator();
	//コマンドリストの生成
	commandList_ = MakeCommandList();
	//コマンドキューの生成
	commandQueue_ = MakeCommandQueue();
}

//スワップチェーンの生成
void DirectXBase::CreateSwapChain() {
	//スワップチェーンの生成
	swapChain_ = MakeSwapChain();
}

// 深度バッファの生成
void DirectXBase::CreateDepthBuffer() {
	depthStencilResource_ = MakeDepthStencilTextureResource(WinApi::kClientWidth, WinApi::kClientHeight);
}

//各種デスクリプターヒープの生成
void DirectXBase::CreateDescriptorHeap() {
	//DescriptorSize
	descriptorSizeRTV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	descriptorSizeSRV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descriptorSizeDSV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	//CreateDescriptorHeap
	//RTV
	rtvDescriptorHeap_ = MakeDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	//SRV用のヒープでデスクリプタの数が128。SRVはShaderを触るものなので、ShaderVisibleはtrue
	srvDescriptorHeap_ = MakeDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount, true);
	//DSV用のヒープでディスクリプタの数は1。DSVはShader内で触れるものではないので、ShaderVisibleはfalse
	dsvDescriptorHeap_ = MakeDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
}

//RTVの初期化
void DirectXBase::InitializeRTV() {
	//SwapChainからResourceを引っ張ってくる
	swapChainResources_[0] = BringResourcesFromSwapChain(0);
	swapChainResources_[1] = BringResourcesFromSwapChain(1);
	//RTV用の設定
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//出力結果をSRGBに変換して書き込む
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;//2dテクスチャとして書き込む
	//RTVハンドルの要素数を2個に変更する
	rtvHandles_.resize(2);
	//ディスクリプタの先頭を取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	//まずは1つ目を作る。1つ目は最初のところに作る。作る場所をこちらで指定してあげる必要がある
	rtvHandles_[0] = rtvStartHandle;
	//2つ目ディスクリプタハンドルを得る(自力で)
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + descriptorSizeRTV_;
	for (uint32_t i = 0; i < rtvHandles_.size(); i++) {
		//レンダーターゲットビューの生成
		device_->CreateRenderTargetView(swapChainResources_[i].Get(), &rtvDesc_, rtvHandles_[i]);
	}
}

//深度ステンシルビューの初期化
void DirectXBase::InitializeDepthStencil() {
	//Depthの機能を有効化
	depthStencilDesc_.DepthEnable = true;
	//書き込みをする
	depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	//DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;//2dTexture
	//DSVHeapの先頭にDSVを作る
	device_->CreateDepthStencilView(depthStencilResource_.Get(), &dsvDesc, dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());
}

//フェンスの初期化
void DirectXBase::InitializeFence() {
	fence_ = MakeFence();
}

//ビューポート矩形の初期化
void DirectXBase::InitializeViewport() {
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport_.Width = WinApi::kClientWidth;
	viewport_.Height = WinApi::kClientHeight;
	viewport_.TopLeftX = 0.0f;
	viewport_.TopLeftY = 0.0f;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;
}

//シザリング矩形の初期化
void DirectXBase::InitializeScissorRect() {
	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect_.left = 0;
	scissorRect_.right = WinApi::kClientWidth;
	scissorRect_.top = 0;
	scissorRect_.bottom = WinApi::kClientHeight;

}

//DXCコンパイラの生成
void DirectXBase::CreateDXCCompiler() {
	HRESULT result = S_FALSE;
	//DXCユーティリティの生成
	result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(result));
	//DXCコンパイラの生成
	result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(result));
	//デフォルトインクルードハンドラの生成
	result = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(result));
}

//ImGuiの初期化
void DirectXBase::InitializeImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApi_->GetHwnd());
	ImGui_ImplDX12_Init(
		device_.Get(),
		swapChainDesc_.BufferCount,
		rtvDesc_.Format,
		srvDescriptorHeap_.Get(),
		GetSRVCPUDescriptorHandle(0),
		GetSRVGPUDescriptorHandle(0));
}

// フレームの開始
void DirectXBase::PreDraw() {
	/*コマンドを積む*/
	//これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

	//今回のバリアはTransition
	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier_.Transition.pResource = swapChainResources_[backBufferIndex].Get();
	//遷移前(現在)のResourceState
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のResourceState
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier_);
	//描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = DirectXBase::GetCPUDescriptorHandle(dsvDescriptorHeap_.Get(), descriptorSizeDSV_, 0);
	//描画先のRTVを設定する
	commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex], false, &dsvHandle);
	//指定した色で画面をクリアする
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };//青っぽい色。RGBAの順
	commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex], clearColor, 0, nullptr);
	//指定した深度で画面全体をクリアする
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	//描画用のDescriptorHeapの設定
	ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvDescriptorHeap_.Get() };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());
	//ビューポート領域を設定する
	commandList_->RSSetViewports(1, &viewport_);
	//シザ－矩形の設定
	commandList_->RSSetScissorRects(1, &scissorRect_);
}

// フレームの終了
void DirectXBase::PostDraw() {
	HRESULT result = S_FALSE;
	//これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();
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
	ComPtr<ID3D12CommandList> commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists->GetAddressOf());
	//GPUとOSに画面の交換を行うように通知
	swapChain_->Present(1, 0);
	//GPUがここまでたどり着いた時に、Fenceの値を指定した値に代入するようにSignalを送る
	commandQueue_->Signal(fence_.Get(), ++fenceValue_);
	if (fence_->GetCompletedValue() < fenceValue_) {
		//指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		//イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);
	}
	//FPS固定
	FixFPS::GetInstance()->Update();
	//次のフレーム用のコマンドリストを準備
	//コマンドアローケータのリセット
	result = commandAllocator_->Reset();
	assert(SUCCEEDED(result));
	//コマンドリストのリセット
	result = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(result));
}

//終了処理
void DirectXBase::Finalize(){
	//オブジェクトの開放
	CloseHandle(fenceEvent_);
}

//深度バッファリソースの生成
ComPtr<ID3D12Resource> DirectXBase::MakeDepthStencilTextureResource(int32_t width, int32_t height) {
	//生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;//Textureの横幅
	resourceDesc.Height = height;//Textureの縦幅
	resourceDesc.MipLevels = 1;//mipmapの数
	resourceDesc.DepthOrArraySize = 1;//奥行or配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;//サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;//2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//DepthStencilとして使う通知

	//利用するHeap設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;//VRAM上に作る

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;//1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//フォーマット。Resourceと合わせる

	//Resourceの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device_->CreateCommittedResource(
		&heapProperties,//Heapの設定
		D3D12_HEAP_FLAG_NONE,//Heapの特殊な設定。特になし
		&resourceDesc,//Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値を書き込む状態にしておく
		&depthClearValue,//Clear最適値
		IID_PPV_ARGS(&resource));//作成するResourceポインタのポインタ
	assert(SUCCEEDED(hr));
	return resource;
}

// ディスクリプタヒープの生成
ComPtr<ID3D12DescriptorHeap> DirectXBase::MakeDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool	shaderVisible) {
	ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = device_->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));
	return descriptorHeap;
}

//シェーダーのコンパイル
ComPtr<IDxcBlob> DirectXBase::CompilerShader(const std::wstring& filePath, const wchar_t* profile) {
	//1. hlslファイルを読み込む
		//これからシェーダーをコンパイルする旨をログに出す
	Log::ConsolePrintf(Log::ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
	//hlslファイルを読み込む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils_->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読み込めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;//UTF8の文字コードであることを通知
	//2. Compilerする
	LPCWSTR arguments[] = {
		filePath.c_str(),//コンパイル対象のhlslファイル名
		L"-E",L"main",//エントリーポイントの指定。基本的にmain以外にはしない
		L"-T",profile,//ShaderProfileの設定
		L"-Zi",L"-Qembed_debug",//デバック用の情報を埋め込む
		L"-Od",//最適化を外しておく
		L"-Zpr",//メモリレイアウトは行優先
	};
	//実際にShaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcResult> shaderResult = nullptr;
	hr = dxcCompiler_->Compile(
		&shaderSourceBuffer,//読み込んだファイル
		arguments,//コンパイルオプション
		_countof(arguments),//コンパイルオプションの数
		includeHandler_.Get(),//includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)//コンパイル結果
	);
	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));
	//3. 警告・エラーが出ていないか確認する
	Microsoft::WRL::ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log::ConsolePrintf(shaderError->GetStringPointer());
		//警告・エラーダメゼッタイ
		assert(false);
	}
	//4. Compiler結果を受け取って返す
	//コンパイル結果から実行用のバイナリ部分を取得
	Microsoft::WRL::ComPtr<IDxcBlob> shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したらログを出す
	Log::ConsolePrintf(Log::ConvertString(std::format(L"Compile Succeeded, path:{},profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却
	return shaderBlob;
}

//バッファリソースの生成
ComPtr<ID3D12Resource> DirectXBase::CreateBufferResource(size_t sizeInBytes) {
	//リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperities{};
	uploadHeapProperities.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う
	//リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeInBytes;//リソースのサイズ。
	//バッファの場合はこれらは1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際にリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device_->CreateCommittedResource(&uploadHeapProperities, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));
	return resource;
}

// テクスチャリソースの生成
ComPtr<ID3D12Resource> DirectXBase::CreateTextureResource(const DirectX::TexMetadata& metadada) {
	//1.metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadada.width);//Textureの幅
	resourceDesc.Height = UINT(metadada.height);//Textureの高さ
	resourceDesc.MipLevels = UINT16(metadada.mipLevels);//mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadada.arraySize);//奥行or配列Textureの配列数
	resourceDesc.Format = metadada.format;//TextureのFormat
	resourceDesc.SampleDesc.Count = 1;//サンプリング。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadada.dimension);//Textureの次元数。普段使っているのは2次元
	//2.利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperities{};
	heapProperities.Type = D3D12_HEAP_TYPE_CUSTOM;//細かい設定を行う
	heapProperities.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//WriteBackポリシーでCPUアクセス可能
	heapProperities.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//プロセッサの近くに配置
	//3.Resourceを生成する
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device_->CreateCommittedResource(
		&heapProperities,//Heapの設定
		D3D12_HEAP_FLAG_NONE,//Heapの特殊な設定。特になし
		&resourceDesc,//Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,//初回のResourceState。Textureは基本読むだけ
		nullptr,//Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource)//作成するResourceポインタへのポインタ
	);
	assert(SUCCEEDED(hr));
	return resource;
}

// TextureResourceにデータを転送する 
ComPtr<ID3D12Resource> DirectXBase::UploadTextureData(ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages) {
	std::vector<D3D12_SUBRESOURCE_DATA>subResources;
	//PrePareUploadを利用して読み込んだデータからDirectX12用のSubresourceの配列を作成する
	DirectX::PrepareUpload(device_.Get(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subResources);
	//Subresourceの数を基にコピー元となるIntermediateResourceに必要なサイズを計算する
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subResources.size()));
	//計算したサイズでIntermediateResourceを作成する
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = CreateBufferResource(intermediateSize);
	//UpdateSubresourcesを利用して、IntermediateResourceにSubresourceのデータを書き込み、textureに転送するコマンドを積む
	UpdateSubresources(commandList_.Get(), texture.Get(), intermediateResource.Get(), 0, 0, UINT(subResources.size()), subResources.data());
	//Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	commandList_.Get()->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

// SRVの指定番号のCPUデスクリプタハンドルを取得する
D3D12_CPU_DESCRIPTOR_HANDLE DirectXBase::GetSRVCPUDescriptorHandle(uint32_t index) {
	return GetCPUDescriptorHandle(srvDescriptorHeap_, descriptorSizeSRV_, index);
}

// SRVの指定番号のGPUデスクリプタハンドルを取得する
D3D12_GPU_DESCRIPTOR_HANDLE DirectXBase::GetSRVGPUDescriptorHandle(uint32_t index) {
	return GetGPUDescriptorHandle(srvDescriptorHeap_, descriptorSizeSRV_, index);
}

// RTVの指定番号のCPUデスクリプタハンドルを取得する
D3D12_CPU_DESCRIPTOR_HANDLE DirectXBase::GetRTVCPUDescriptorHandle(uint32_t index) {
	return  GetCPUDescriptorHandle(rtvDescriptorHeap_, descriptorSizeRTV_, index);
}

// RTVの指定番号のGPUデスクリプタハンドルを取得する
D3D12_GPU_DESCRIPTOR_HANDLE DirectXBase::GetRTVGPUDescriptorHandle(uint32_t index) {
	return  GetGPUDescriptorHandle(rtvDescriptorHeap_, descriptorSizeRTV_, index);
}

// DSVの指定番号のCPUデスクリプタハンドルを取得する
D3D12_CPU_DESCRIPTOR_HANDLE DirectXBase::GetDSVCPUDescriptorHandle(uint32_t index) {
	return  GetCPUDescriptorHandle(dsvDescriptorHeap_, descriptorSizeDSV_, index);
}

// DSVの指定番号のGPUデスクリプタハンドルを取得する
D3D12_GPU_DESCRIPTOR_HANDLE DirectXBase::GetDSVGPUDescriptorHandle(uint32_t index) {
	return  GetGPUDescriptorHandle(dsvDescriptorHeap_, descriptorSizeDSV_, index);
}

//デバイスのゲッター
ID3D12Device* DirectXBase::GetDevice() const {
	return device_.Get();
}

//コマンドリストのゲッター
ID3D12GraphicsCommandList* DirectXBase::GetCommandList() const {
	return commandList_.Get();
}

// デスクリプターCPUハンドルのゲッター
D3D12_CPU_DESCRIPTOR_HANDLE  DirectXBase::GetCPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

// デスクリプターGPUハンドルのゲッター
D3D12_GPU_DESCRIPTOR_HANDLE DirectXBase::GetGPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

// IDXIファクトリーの生成
ComPtr<IDXGIFactory7> DirectXBase::MakeIDXGIFactory() {
	HRESULT result = S_FALSE;
	ComPtr<IDXGIFactory7> factory = nullptr;
	result = CreateDXGIFactory(IID_PPV_ARGS(&factory));
	assert(SUCCEEDED(result));//生成できなかった場合止める
	return factory;
}

//使用するアダプタを決定
ComPtr<IDXGIAdapter4> DirectXBase::DecideUseAdapter() {
	HRESULT result = S_FALSE;
	ComPtr<IDXGIAdapter4> adapter = nullptr;
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
ComPtr<ID3D12Device> DirectXBase::MakeD3D12Device() {
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
		result = D3D12CreateDevice(useAdapter_.Get(), featureLevels[i], IID_PPV_ARGS(&device));
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
ComPtr<ID3D12CommandQueue> DirectXBase::MakeCommandQueue() {
	HRESULT result = S_FALSE;
	ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	result = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	//コマンドキューの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));
	return commandQueue;
}

// コマンドアローケータの生成
ComPtr<ID3D12CommandAllocator> DirectXBase::MakeCommandAllocator() {
	HRESULT result = S_FALSE;
	ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
	result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	//コマンドアローケータがうまく生成できなかった場合止める
	assert(SUCCEEDED(result));
	return commandAllocator;
}

// コマンドリストの生成
ComPtr<ID3D12GraphicsCommandList> DirectXBase::MakeCommandList() {
	HRESULT result = S_FALSE;
	ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
	result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList));
	//コマンドリストがうまく生成できなかった場合止める
	assert(SUCCEEDED(result));
	return commandList;
}

//スワップチェーンの生成
ComPtr<IDXGISwapChain4> DirectXBase::MakeSwapChain() {
	HRESULT result = S_FALSE;
	swapChainDesc_.Width = WinApi::kClientWidth;//画面の横幅
	swapChainDesc_.Height = WinApi::kClientHeight;//画面の縦幅
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色形式
	swapChainDesc_.SampleDesc.Count = 1;//マルチサンプルしない
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//描画のターゲットとして利用
	swapChainDesc_.BufferCount = 2;//ダブルバッファ
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//モニタにうつしたら、中身を破棄
	ComPtr<IDXGISwapChain4> swapChain = nullptr;
	//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	result = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_.Get(), winApi_->GetHwnd(), &swapChainDesc_, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf()));
	assert(SUCCEEDED(result));
	return swapChain;
}

// SwapChainからResourceを引っ張ってくる
ComPtr<ID3D12Resource> DirectXBase::BringResourcesFromSwapChain(UINT num) {
	HRESULT result = S_FALSE;
	ComPtr<ID3D12Resource> swapChainResource = nullptr;
	result = swapChain_->GetBuffer(num, IID_PPV_ARGS(&swapChainResource));
	//うまく取得できなければ起動できない
	assert(SUCCEEDED(result));
	return swapChainResource;
}

//Fenceを作成する
ComPtr<ID3D12Fence> DirectXBase::MakeFence() {
	HRESULT result = S_FALSE;
	ComPtr<ID3D12Fence> fence = nullptr;
	fenceValue_ = 0;
	result = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(result));

	//FenceのSignalを待つためのイベントを作成
	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent_ != nullptr);
	return fence;
}

//デバックレイヤー
void DirectXBase::DebugLayer() {
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
void DirectXBase::StopExecution() {
#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
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
