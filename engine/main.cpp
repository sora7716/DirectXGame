#include "base/DirectXCommon.h"
#include "math/Matrix4x4.h"
#include "math/func/Math.h"
#include "math/rendering/Rendering.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

/// <summary>
/// CompilerShader関数
/// </summary>
/// <param name="filePaht">CompilerするShaderファイルへのパス</param>
/// <param name="profile">Compilerに使用するProfile</param>
/// <param name="dxcUtils">初期化で生成したもの</param>
/// <param name="dxcCompiler">初期化で生成したもの</param>
/// <param name="includeHandler">初期化で生成したもの</param>
/// <returns></returns>
IDxcBlob* CompilerShader(
	//CompilerするShaderファイルへのパス
	const std::wstring& filePath,
	//Compilerに使用するProfile
	const wchar_t* profile,
	//初期化で生成したものを3つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler
) {
	//1. hlslファイルを読み込む
	//これからシェーダーをコンパイルする旨をログに出す
	Log::ConsolePrintf(Log::ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
	//hlslファイルを読み込む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
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
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,//読み込んだファイル
		arguments,//コンパイルオプション
		_countof(arguments),//コンパイルオプションの数
		includeHandler,//includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)//コンパイル結果
	);
	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));
	//3. 警告・エラーが出ていないか確認する
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log::ConsolePrintf(shaderError->GetStringPointer());
		//警告・エラーダメゼッタイ
		assert(false);
	}
	//4. Compiler結果を受け取って返す
	//コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
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

/// <summary>
/// Resourceの生成
/// </summary>
/// <param name="device">デバイス</param>
/// <param name="sizeInBytes">バイト</param>
/// <returns>リソース</returns>
ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
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
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(&uploadHeapProperities, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));
	return resource;
}

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//ウィンドウズアプリケーション
	WinApp* winApp = new WinApp();
	//DirectXCommon
	DirectXCommon* directXCommon = new DirectXCommon();

	//ウィンドウの作成
	winApp->CreateGameWindow();
	//デバックレイヤー
	directXCommon->DebugLayer();
	//DirectX12の初期化
	directXCommon->InitializeDirectX12(winApp);

	//SRV用のヒープでデスクリプタの数が128。SRVはShaderを触るものなので、ShaderVisibleはtrue
	ID3D12DescriptorHeap* srvDescriptorHeap = directXCommon->MakeDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);

	//dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	HRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr));

	//現時点でincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//RootParameterの作成。複数設定できるので配列。
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使うb0のbと一致する
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderを使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインドb0の0と一致する

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderを使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号

	descriptionRootSignature.pParameters = rootParameters;//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);//配列の長さ


	//シリアライズしてバイナリにする
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log::ConsolePrintf(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//バイナリを元に作成
	ID3D12RootSignature* rootSignature = nullptr;
	hr = directXCommon->device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	//InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計周り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//Shaderをコンパイルする
	//VertexShader
	IDxcBlob* vertexShaderBlob = CompilerShader(L"engine/resource/shader/Object3d.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob != nullptr);

	//PixelShader
	IDxcBlob* pixelShaderBlob = CompilerShader(L"engine/resource/shader/object3d.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob != nullptr);

	//PSOを生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature;//RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;//InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };//VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };//VertexShader
	graphicsPipelineStateDesc.BlendState = blendDesc;//BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;//RasterizerState
	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//どのように画面に色を打ち込むかの設定(気にしなくてよい)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//実際に生成
	ID3D12PipelineState* graphicsPipelineState = nullptr;
	hr = directXCommon->device_->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));

	//頂点リソースを生成
	ID3D12Resource* vertexResource = CreateBufferResource(directXCommon->device_, sizeof(Vector4) * 3);

	//マテリアル用のリソースを作る
	ID3D12Resource* materialResource = CreateBufferResource(directXCommon->device_, sizeof(Vector4));
	//マテリアルにデータを書き込む
	Vector4* materialData = nullptr;
	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//色を書き込む
	*materialData = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

	//WVP用のリソースを作る
	ID3D12Resource* wvpResource = CreateBufferResource(directXCommon->device_, sizeof(Matrix4x4));
	//データを書き込む
	Matrix4x4* wvpData = nullptr;
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	//単位行列を書き込んでおく
	*wvpData = Math::MakeIdentity4x4();

	//VertexBufferViewを作成する(頂点バッファービュー)
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(Vector4) * 3;
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(Vector4);

	//頂点リソースにデータを書き込む
	Vector4* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0] = { -0.5f,-0.5f,0.0f,1.0f };
	//上
	vertexData[1] = { 0.0f,0.5f,0.0f,1.0f };
	//右下
	vertexData[2] = { 0.5f,-0.5f,0.0f,1.0f };

	//ビューポート
	D3D12_VIEWPORT viewport{};
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = WinApp::kClientWidth;
	viewport.Height = WinApp::kClientHeight;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//シーザー矩形
	D3D12_RECT scissorRect{};
	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WinApp::kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WinApp::kClientHeight;

	//GameObjectの初期化
	//Transform変数を作成
	Transform transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	//カメラ
	Transform cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f,},{0.0f,0.0f,-5.0f} };

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp->GetHwnd());
	ImGui_ImplDX12_Init(
		directXCommon->device_,
		directXCommon->swapChainDesc_.BufferCount,
		directXCommon->rtvDesc_.Format,
		srvDescriptorHeap, 
		srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

	//ウィンドウの✖ボタンが押されるまでループ
	while (true) {
		if (winApp->ProcesMessage()) {
			break;
		}
		//ImGuiの開始
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		//更新処理
		transform.rotate.y += 0.03f;
		Matrix4x4 worldMatrix = Rendering::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
		Matrix4x4 cameraMatrix = Rendering::MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
		Matrix4x4 viewMatrix = ~cameraMatrix;
		Matrix4x4 projectionMatrix = Rendering::MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = worldMatrix * viewMatrix * projectionMatrix;
		*wvpData = worldViewProjectionMatrix;
		//開発用のUIの処理
		ImGui::ShowDemoWindow();
		//ImGuiの内部コマンドを生成する
		ImGui::Render();
		//描画処理
		directXCommon->DrawBeginFrame();
		//描画用のDescriptorHeapの設定
		ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap };
		directXCommon->commandList_->SetDescriptorHeaps(1, descriptorHeaps);
		//描画するコマンドを積む
		directXCommon->commandList_->RSSetViewports(1, &viewport);
		directXCommon->commandList_->RSSetScissorRects(1, &scissorRect);
		//RootSignatureを設定。PSOに設定しているけど別途設定が必要
		directXCommon->commandList_->SetGraphicsRootSignature(rootSignature);
		directXCommon->commandList_->SetPipelineState(graphicsPipelineState);
		directXCommon->commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);//VBVを設定
		//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
		directXCommon->commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//マテリアルCBufferの場所を設定
		directXCommon->commandList_->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
		//wvp用のCBufferの場所を設定
		directXCommon->commandList_->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
		//描画!(DrawCall/ドローコール)。3頂点で1つのインスタンス。
		directXCommon->commandList_->DrawInstanced(3, 1, 0, 0);
		//実際のcommandListのImGuiの描画コマンドを積む
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXCommon->commandList_);
		//描画の終了
		directXCommon->DrawEndFrame();
	}

	//ImGuiの終了処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	srvDescriptorHeap->Release();
	vertexResource->Release();
	graphicsPipelineState->Release();
	signatureBlob->Release();
	if (errorBlob) {
		errorBlob->Release();
	}
	rootSignature->Release();
	pixelShaderBlob->Release();
	vertexShaderBlob->Release();
	materialResource->Release();
	wvpResource->Release();
	delete winApp;
	delete directXCommon;
	//リソースリークチェック
	IDXGIDebug1* debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}
	return 0;
}