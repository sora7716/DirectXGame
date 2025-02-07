#include "base/DirectXCommon.h"
#include "math/Matrix4x4.h"
#include "math/func/Math.h"
#include "math/ResourceData.h"
#include "math/rendering/Rendering.h"
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/d3dx12.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>

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
/// <summary>
/// Textureデータを読み込む
/// </summary>
/// <param name="filePath">ファイルパス</param>
/// <returns></returns>
DirectX::ScratchImage LoadTexture(const std::string& filePath) {
	//テクスチャファイルを読み込んでプログラムを扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = Log::ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//ミップマップ付きのデータを返す
	return mipImages;
}

/// <summary>
/// DirectX12のTextureResourceを作る
/// </summary>
/// <param name="device">デバイス</param>
/// <param name="metadada">メタデータ</param>
/// <returns>TextureResource</returns>
ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadada) {
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
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
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
/// <summary>
/// TextureResourceにデータを転送する 
/// </summary>
/// <param name="texture">テクスチャ</param>
/// <param name="mipImages">ミップマップ</param>
/// <param name="device">デバイス</param>
/// <param name="commandList">コマドリスト</param>
/// <returns></returns>
[[nodiscard]]//属性という機能(戻り値を破棄してはならない)むやみやたらとつけてはいけない
ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, ID3D12Device* device, ID3D12GraphicsCommandList* commandList) {
	std::vector<D3D12_SUBRESOURCE_DATA>subResources;
	//PrePareUploadを利用して読み込んだデータからDirectX12用のSubresourceの配列を作成する
	DirectX::PrepareUpload(device, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subResources);
	//Subresourceの数を基にコピー元となるIntermediateResourceに必要なサイズを計算する
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subResources.size()));
	//計算したサイズでIntermediateResourceを作成する
	ID3D12Resource* intermediateResource = CreateBufferResource(device, intermediateSize);
	//UpdateSubresourcesを利用して、IntermediateResourceにSubresourceのデータを書き込み、textureに転送するコマンドを積む
	UpdateSubresources(commandList, texture, intermediateResource, 0, 0, UINT(subResources.size()), subResources.data());
	//Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	commandList->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

/// <summary>
/// DepthStencilTextureを作る
/// </summary>
/// <param name="device">デバイス</param>
/// <param name="width">横幅</param>
/// <param name="height">縦幅</param>
/// <returns>DepthStencilTexture</returns>
ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height) {
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
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,//Heapの設定
		D3D12_HEAP_FLAG_NONE,//Heapの特殊な設定。特になし
		&resourceDesc,//Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値を書き込む状態にしておく
		&depthClearValue,//Clear最適値
		IID_PPV_ARGS(&resource));//作成するResourceポインタのポインタ
	assert(SUCCEEDED(hr));
	return resource;
}

/// <summary>
/// デスクリプターCPUハンドルのゲッター
/// </summary>
/// <param name="descriptorHeap">デスクリプターヒープ</param>
/// <param name="descriptorSize">デスクリプターサイズ</param>
/// <param name="index">インデックス</param>
/// <returns>デスクリプターCPUハンドル</returns>
D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

/// <summary>
/// デスクリプターGPUハンドルのゲッター
/// </summary>
/// <param name="descriptorHeap">デスクリプターヒープ</param>
/// <param name="descriptorSize">デスクリプターサイズ</param>
/// <param name="index">インデックス</param>
/// <returns>デスクリプターGPUハンドル</returns>
D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

//マテリアルデータ
typedef struct MaterialData {
	std::string textureFilePath;
}MaterialData;

/// <summary>
/// LoadMaterialTemplateFile
/// </summary>
/// <param name="directoryPath">ディレクトリ名</param>
/// <param name="filename">ファイル名</param>
/// <returns>マテリアのファイル名</returns>
MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	//1.中で必要となる変数の宣言
	MaterialData materialData;//構築するMaterialData
	std::string line;//ファイルから読んだ1行を格納するもの
	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);//ファイルを開く
	assert(file.is_open());//とりあえず開かなかったら止める
	//3.実際にファイルを読み込み、MaterialDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;
		//identifilerに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			//連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}
	//4.MaterialDataを返す
	return materialData;
}


//モデルデータの構造体
typedef struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
}ModelData;

/// <summary>
/// objファイルの読み込み
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="filename">ファイル名</param>
/// <returns>obj</returns>
ModelData LoadObjeFile(const std::string& directoryPath, const std::string& filename) {
	//1.中で必要な変数を宣言
	ModelData modelData;//構築するModelData
	std::vector<Vector4>positions;//位置
	std::vector<Vector3>normals;//法線
	std::vector<Vector2>texcoords;//テクスチャ座標
	std::string line;//ファイルから呼んだ1桁を格納するもの
	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);//ファイルを開く
	assert(file.is_open());//とりあえず開かなかったら止める
	//3.実際にファイルを読み込み、ModelDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;//先頭の識別子を読む
		//identifierに応じた処理
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.x *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
			//面は三角形限定。そのほかは未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				//頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3]{};
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');// /区切りで読み込んでいく
					elementIndices[element] = std::stoi(index);
				}
				//要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				/*VertexData vertex = { position,texcoord,normal };
				modelData.vertices.push_back(vertex);*/
				triangle[faceVertex] = { position,texcoord,normal };
			}
			//頂点を逆順で登録することで、周りを逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			//materialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			//基本的にobjファイルと同一の階層にmtlは存在させるので、ディレクトリ名とファイル名を返す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}
	//4.ModelDataを返す
	return modelData;
}

/// <summary>
/// リークチェッカー
/// </summary>
struct D3DResourceLeakChacker {
	~D3DResourceLeakChacker() {
		//リソースリークチェック
		IDXGIDebug1* debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
			debug->Release();
		}
	}
};

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakChacker leakChacker;
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	//ウィンドウズアプリケーション
	std::unique_ptr<WinApp> winApp = std::make_unique<WinApp>();
	//DirectXCommon
	std::unique_ptr<DirectXCommon> directXCommon = std::make_unique<DirectXCommon>();

	//ウィンドウの作成
	winApp->CreateGameWindow();
	//デバックレイヤー
	directXCommon->DebugLayer();
	//DirectX12の初期化
	directXCommon->InitializeDirectX12(winApp.get());

	//SRV用のヒープでデスクリプタの数が128。SRVはShaderを触るものなので、ShaderVisibleはtrue
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap = directXCommon->MakeDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);

	//dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
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
	//Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//バイナリフィルター
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//0~1の範囲外をリピート
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//0~1の範囲外をリピート
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;//比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;//ありたっけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0;//レジスタ番号
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderを使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	//DescriptorRange
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//0から始まる
	descriptorRange[0].NumDescriptors = 1;//数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	//RootParameterの作成。複数設定できるので配列。
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	//色情報
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使うb0のbと一致する	
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderを使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインドb0の0と一致する

	//Transform
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderを使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号

	//DescriptorTable(DescriptorRangeをまとめたもの)
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderを使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	//ライト
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderを使う
	rootParameters[3].Descriptor.ShaderRegister = 1;//レジスタ番号1を使う

	descriptionRootSignature.pParameters = rootParameters;//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);//配列の長さ

	//シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log::ConsolePrintf(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//バイナリを元に作成
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	hr = directXCommon->device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	//InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};

	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

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
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = CompilerShader(L"engine/resource/shader/Object3d.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob != nullptr);

	//PixelShader
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = CompilerShader(L"engine/resource/shader/object3d.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob != nullptr);

	//DepthStencilTextureをウィンドウのサイズで作成
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = CreateDepthStencilTextureResource(directXCommon->device_.Get(), winApp->kClientWidth, winApp->kClientHeight);
	//DSV用のヒープでディスクリプタの数は1。DSVはShader内で触れるものではないので、ShaderVisibleはfalse
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap = directXCommon->MakeDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
	//DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;//2dTexture
	//DSVHeapの先頭にDSVを作る
	directXCommon->device_->CreateDepthStencilView(depthStencilResource.Get(), &dsvDesc, dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化
	depthStencilDesc.DepthEnable = true;
	//書き込みをする
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	//PSOを生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();//RootSignature
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
	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//実際に生成
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;
	hr = directXCommon->device_->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));

	//マテリアル用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = CreateBufferResource(directXCommon->device_.Get(), sizeof(Material));
	//マテリアルにデータを書き込む
	Material* materialData = nullptr;
	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//色を書き込む
	materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData->enableLighting = true;
	materialData->uvTransform = Math::MakeIdentity4x4();

	//Spriteマテリアル用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceSprite = CreateBufferResource(directXCommon->device_.Get(), sizeof(Material));
	//マテリアルにデータを書き込む
	Material* materialDataSprite = nullptr;
	//書き込むためのアドレスを取得
	materialResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite));
	//色を書き込む
	materialDataSprite->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialDataSprite->enableLighting = false;
	materialDataSprite->uvTransform = Math::MakeIdentity4x4();

	//WVP用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource = CreateBufferResource(directXCommon->device_.Get(), sizeof(TransformationMatrix));
	//データを書き込む
	TransformationMatrix* wvpData = nullptr;
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	//単位行列を書き込んでおく
	wvpData->WVP = Math::MakeIdentity4x4();
	wvpData->World = Math::MakeIdentity4x4();

	const int kSubdivision = 16;
	const int kSphereVertexNum = kSubdivision * kSubdivision * 6;

	//モデルデータの読み込み
	ModelData modelData = LoadObjeFile("engine/resource/base", "plane.obj");

	//Index用(3dGameObject)
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource = CreateBufferResource(directXCommon->device_.Get(), sizeof(uint32_t) * modelData.vertices.size());
	//Indexに対応したViewを作成するIndexBufferView(IBV)
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	//リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView.SizeInBytes = UINT(sizeof(uint32_t) * modelData.vertices.size());
	//インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	//IndexResourceにデータを書き込む
	uint32_t* indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	//頂点リソースを生成
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = CreateBufferResource(directXCommon->device_.Get(), sizeof(VertexData) * modelData.vertices.size());
	//VertexBufferViewを作成する(頂点バッファービュー)
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());//頂点データをリソースにコピー

	for (int i = 0; i < modelData.vertices.size(); i++) {
		indexData[i] = i; indexData[i + 1] = i + 1; indexData[i + 2] = i + 2;
		indexData[i + 3] = i + 1; indexData[i + 4] = i + 3; indexData[i + 5] = i + 2;
	}

	/*for (int i = 0; i < modelData.vertices.size() / 6; i++) {
		indexData[i] = i; indexData[i + 1] = i + 1; indexData[i + 2] = i + 2;
		indexData[i + 3] = i + 1; indexData[i + 4] = i + 3; indexData[i + 5] = i + 2;
	}*/
	////経度分割1つ分の角度φd
	//const float kLonEvery = pi_f * 2.0f / float(kSubdivision);
	////緯度分割1つ分の角度θd
	//const float kLatEvery = pi_f / float(kSubdivision);
	////緯度の方向に分割
	//for (int latIndex = 0; latIndex < kSubdivision; ++latIndex) {
	//	float lat = -pi_f / 2.0f + kLatEvery * latIndex;//θ
	//	//経度の方向に分割しながら線を描く
	//	for (int lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
	//		float lon = lonIndex * kLonEvery;//φ
	//		uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
	//		//頂点データを打ち込む
	//		//a(左下)
	//		vertexData[start].position.x = std::cos(lat) * std::cos(lon);
	//		vertexData[start].position.y = std::sin(lat);
	//		vertexData[start].position.z = std::cos(lat) * std::sin(lon);
	//		vertexData[start].position.w = 1.0f;
	//		vertexData[start].texcoord.x = float(lonIndex) / float(kSubdivision);
	//		vertexData[start].texcoord.y = 1.0f - float(latIndex - 1) / float(kSubdivision);
	//		vertexData[start].normal.x = vertexData[start].position.x;
	//		vertexData[start].normal.y = vertexData[start].position.y;
	//		vertexData[start].normal.z = vertexData[start].position.z;
	//		//b(左上)
	//		vertexData[start + 1].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
	//		vertexData[start + 1].position.y = std::sin(lat + kLatEvery);
	//		vertexData[start + 1].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
	//		vertexData[start + 1].position.w = 1.0f;
	//		vertexData[start + 1].texcoord.x = float(lonIndex) / float(kSubdivision);
	//		vertexData[start + 1].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);
	//		vertexData[start + 1].normal.x = vertexData[start + 1].position.x;
	//		vertexData[start + 1].normal.y = vertexData[start + 1].position.y;
	//		vertexData[start + 1].normal.z = vertexData[start + 1].position.z;
	//		//c(右下)
	//		vertexData[start + 2].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
	//		vertexData[start + 2].position.y = std::sin(lat);
	//		vertexData[start + 2].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
	//		vertexData[start + 2].position.w = 1.0f;
	//		vertexData[start + 2].texcoord.x = float(lonIndex + 1) / float(kSubdivision);
	//		vertexData[start + 2].texcoord.y = 1.0f - float(latIndex - 1) / float(kSubdivision);
	//		vertexData[start + 2].normal.x = vertexData[start + 2].position.x;
	//		vertexData[start + 2].normal.y = vertexData[start + 2].position.y;
	//		vertexData[start + 2].normal.z = vertexData[start + 2].position.z;
	//		//b(左上)
	//		vertexData[start + 3] = vertexData[start + 1];
	//		vertexData[start + 3].normal.x = vertexData[start + 3].position.x;
	//		vertexData[start + 3].normal.y = vertexData[start + 3].position.y;
	//		vertexData[start + 3].normal.z = vertexData[start + 3].position.z;
	//		//d(右上)
	//		vertexData[start + 4].position.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
	//		vertexData[start + 4].position.y = std::sin(lat + kLatEvery);
	//		vertexData[start + 4].position.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
	//		vertexData[start + 4].position.w = 1.0f;
	//		vertexData[start + 4].texcoord.x = float(lonIndex + 1) / float(kSubdivision);
	//		vertexData[start + 4].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);
	//		vertexData[start + 4].normal.x = vertexData[start + 4].position.x;
	//		vertexData[start + 4].normal.y = vertexData[start + 4].position.y;
	//		vertexData[start + 4].normal.z = vertexData[start + 4].position.z;
	//		//c(右下)
	//		vertexData[start + 5] = vertexData[start + 2];
	//		vertexData[start + 5].normal.x = vertexData[start + 5].position.x;
	//		vertexData[start + 5].normal.y = vertexData[start + 5].position.y;
	//		vertexData[start + 5].normal.z = vertexData[start + 5].position.z;

	//		indexData[start] = start; indexData[start + 1] = start + 1; indexData[start + 2] = start + 2;
	//		indexData[start + 3] = start + 1; indexData[start + 4] = start + 4; indexData[start + 5] = start + 2;
	//	}
	//}

	//光源のリソースを作成
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource = CreateBufferResource(directXCommon->device_.Get(), sizeof(DirectionalLight));
	DirectionalLight* directionalLightData = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;

	//Sprite用の頂点リソースの作成
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResourceSprite = CreateBufferResource(directXCommon->device_.Get(), sizeof(VertexData) * 6);

	//頂点リソース
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};
	//リソースの先頭アドレスから使う
	vertexBufferViewSprite.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferViewSprite.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点当たりのサイズ
	vertexBufferViewSprite.StrideInBytes = sizeof(VertexData);
	//頂点データを設定する
	VertexData* vertexDataSprite = nullptr;
	vertexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));
	//1枚目の三角形
	vertexDataSprite[0].position = { 0.0f,360.0f,0.0f,1.0f };//左下
	vertexDataSprite[0].texcoord = { 0.0f,1.0f };
	vertexDataSprite[0].normal = {};
	vertexDataSprite[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexDataSprite[1].texcoord = { 0.0f,0.0f };
	vertexDataSprite[1].normal = {};
	vertexDataSprite[2].position = { 640.0f,360.0f,0.0f,1.0f };//右下
	vertexDataSprite[2].texcoord = { 1.0f,1.0f };
	vertexDataSprite[2].normal = {};

	//2枚目の三角形
	vertexDataSprite[3].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexDataSprite[3].texcoord = { 0.0f,0.0f };
	vertexDataSprite[3].normal = {};
	vertexDataSprite[4].position = { 640.0f,0.0f,0.0f,1.0f };//右上
	vertexDataSprite[4].texcoord = { 1.0f,0.0f };
	vertexDataSprite[4].normal = {};
	vertexDataSprite[5].position = { 640.0f,360.0f,0.0f,1.0f };//右下
	vertexDataSprite[5].texcoord = { 1.0f,1.0f };
	vertexDataSprite[5].normal = {};

	//Sprite用のMaterialを作成
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResoruceSprite = CreateBufferResource(directXCommon->device_.Get(), sizeof(TransformationMatrix));
	//データを書き込む
	TransformationMatrix* transformationMatrixDataSprite = nullptr;
	//書き込むためのアドレス
	transformationMatrixResoruceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
	//単位行列を書き込んでおく
	transformationMatrixDataSprite->WVP = Math::MakeIdentity4x4();
	transformationMatrixDataSprite->World = Math::MakeIdentity4x4();

	//Index用(sprite)
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSprite = CreateBufferResource(directXCommon->device_.Get(), sizeof(uint32_t) * 6);
	//Indexに対応したViewを作成するIndexBufferView(IBV)
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};
	//リソースの先頭のアドレスから使う
	indexBufferViewSprite.BufferLocation = indexResourceSprite->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;
	//IndexResourceにデータを書き込む
	uint32_t* indexDataSprite = nullptr;
	indexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
	indexDataSprite[0] = 0; indexDataSprite[1] = 1; indexDataSprite[2] = 2;
	indexDataSprite[3] = 1; indexDataSprite[4] = 4; indexDataSprite[5] = 2;

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
	Transform transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1.7f,-1.0f,0.0f} };
	//テクスチャの切り替え用
	bool useMonsterBall = true;

	//カメラ
	Transform cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f,},{0.0f,0.0f,-10.0f} };
	//スプライト
	Transform transformSprite{ {1.0f,1.0f,1.0f},{0.0f,.0f,0.0f},{0.0f,0.0f,0.0f} };
	// Textureを呼んで転送する
	DirectX::ScratchImage mipImages = LoadTexture("engine/resource/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource = CreateTextureResource(directXCommon->device_.Get(), metadata);
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = UploadTextureData(textureResource.Get(), mipImages, directXCommon->device_.Get(), directXCommon->commandList_.Get());
	//2枚目のTextureを読む
	DirectX::ScratchImage mipImages2 = LoadTexture(modelData.material.textureFilePath);
	const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource2 = CreateTextureResource(directXCommon->device_.Get(), metadata2);
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource2 = UploadTextureData(textureResource2.Get(), mipImages2, directXCommon->device_.Get(), directXCommon->commandList_.Get());

	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//metaData2を基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = metadata2.format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D11テクスチャ
	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);

	//DescriptorSize
	const uint32_t descriptorSizeSRV = directXCommon->device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	const uint32_t descriptorSizeRTV = directXCommon->device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	const uint32_t descriptorSizeDSV = directXCommon->device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = GetCPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV, 1);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = GetGPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV, 1);

	//SRVの生成
	directXCommon->device_->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);

	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = GetCPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV, 2);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = GetGPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV, 2);
	//SRVの生成
	directXCommon->device_->CreateShaderResourceView(textureResource2.Get(), &srvDesc2, textureSrvHandleCPU2);

	D3D12_CPU_DESCRIPTOR_HANDLE imGuiSrvHandleCPU = GetCPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV, 0);
	D3D12_GPU_DESCRIPTOR_HANDLE imGuiSrvHandleGPU = GetGPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV, 0);

	DirectionalLight directionalLight;
	directionalLight.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight.direction = { 0.0f,-1.0f,0.0f };
	directionalLight.intensity = 1.0f;

	//UVTransform用の変数
	Transform uvTransformSprite{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	Transform uvTransform{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp->GetHwnd());
	ImGui_ImplDX12_Init(
		directXCommon->device_.Get(),
		directXCommon->swapChainDesc_.BufferCount,
		directXCommon->rtvDesc_.Format,
		srvDescriptorHeap.Get(),
		imGuiSrvHandleCPU,
		imGuiSrvHandleGPU);

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
		//transform.rotate.y += 0.001f;
		TransformationMatrix sphereWvpData;
		sphereWvpData.World = Rendering::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
		Matrix4x4 cameraMatrix = Rendering::MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
		Matrix4x4 viewMatrix = ~cameraMatrix;
		Matrix4x4 projectionMatrix = Rendering::MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
		sphereWvpData.WVP = sphereWvpData.World * viewMatrix * projectionMatrix;
		*wvpData = sphereWvpData;
		//Sprite用のWorldViewProjection
		TransformationMatrix spriteWvpData;
		spriteWvpData.World = Rendering::MakeAffineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
		Matrix4x4 viewMatrixSprite = Math::MakeIdentity4x4();
		Matrix4x4 projectionMatrixSprite = Rendering::MakeOrthographicMatrix(0.0f, 0.0f, (float)WinApp::kClientWidth, (float)WinApp::kClientHeight, 0.0f, 100.0f);
		spriteWvpData.WVP = spriteWvpData.World * viewMatrix * projectionMatrixSprite;
		*transformationMatrixDataSprite = spriteWvpData;
		//ライト
		directionalLight.direction = Math::Normalize(directionalLight.direction);
		*directionalLightData = directionalLight;
		//UVTransform
		Matrix4x4 uvTransformMatrixSprite = Rendering::MakeScaleMatrix(uvTransformSprite.scale);
		uvTransformMatrixSprite = uvTransformMatrixSprite * Rendering::MakeRotateZMatrix(uvTransformSprite.rotate.z);
		uvTransformMatrixSprite = uvTransformMatrixSprite * Rendering::MakeTranslateMatrix(uvTransformSprite.translate);
		materialDataSprite->uvTransform = uvTransformMatrixSprite;
		Matrix4x4 uvTransformMatrix = Rendering::MakeScaleMatrix(uvTransform.scale) * Rendering::MakeRotateZMatrix(uvTransform.rotate.z) * Rendering::MakeTranslateMatrix(uvTransform.translate);
		materialData->uvTransform = uvTransformMatrix;
		//開発用のUIの処理
		/*ImGui::ShowDemoWindow();*/
		ImGui::Begin("3dObject");
		ImGui::DragFloat3("scale", &transform.scale.x, 0.1f, 0.0f, 5.0f);
		ImGui::DragFloat3("rotate", &transform.rotate.x, 0.1f);
		ImGui::DragFloat3("translate", &transform.translate.x, 0.1f);
		ImGui::Checkbox("useMonsterBall", &useMonsterBall);
		ImGui::End();

		ImGui::Begin("sprite");
		ImGui::DragFloat3("scale", &transformSprite.scale.x, 0.1f, 0.0f, 5.0f);
		ImGui::DragFloat3("rotate", &transformSprite.rotate.x, 0.1f);
		ImGui::DragFloat3("translate", &transformSprite.translate.x, 0.1f);
		ImGui::End();

		ImGui::Begin("light");
		ImGui::ColorEdit4("color", &directionalLight.color.x);
		ImGui::DragFloat3("direction", &directionalLight.direction.x, 0.1f);
		ImGui::DragFloat("intensity", &directionalLight.intensity, 0.1f, 0.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("UV:sprite");
		ImGui::DragFloat2("scale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::SliderAngle("rotate", &uvTransformSprite.rotate.z);
		ImGui::DragFloat2("translate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("UV:sphere");
		ImGui::DragFloat2("scale", &uvTransform.scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::SliderAngle("rotate", &uvTransform.rotate.z);
		ImGui::DragFloat2("translate", &uvTransform.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::End();
		//ImGuiの内部コマンドを生成する
		ImGui::Render();
		//描画処理
		directXCommon->DrawBeginFrame();
		//ゲームウィンドウの生成
		directXCommon->CreateGameWindow();
		//描画用のDescriptorHeapの設定
		ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap.Get() };
		directXCommon->commandList_->SetDescriptorHeaps(1, descriptorHeaps);
		//描画するコマンドを積む
		directXCommon->commandList_->RSSetViewports(1, &viewport);
		directXCommon->commandList_->RSSetScissorRects(1, &scissorRect);
		//RootSignatureを設定。PSOに設定しているけど別途設定が必要
		directXCommon->commandList_->SetGraphicsRootSignature(rootSignature.Get());
		directXCommon->commandList_->SetPipelineState(graphicsPipelineState.Get());
		directXCommon->commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);//VBVを設定
		directXCommon->commandList_->IASetIndexBuffer(&indexBufferView);//IBVを設定
		//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
		directXCommon->commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//マテリアルCBufferの場所を設定
		directXCommon->commandList_->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
		//wvp用のCBufferの場所を設定
		directXCommon->commandList_->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
		//平行光源CBufferの場所を設定
		directXCommon->commandList_->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
		//SRVのDescriptorTableの先頭を設定	。2はrootParameter[2]のこと
		directXCommon->commandList_->SetGraphicsRootDescriptorTable(2, useMonsterBall ? textureSrvHandleGPU2 : textureSrvHandleGPU);
		//描画先のRTVとDSVを設定する
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = GetCPUDescriptorHandle(dsvDescriptorHeap.Get(), descriptorSizeDSV, 0);
		directXCommon->commandList_->OMSetRenderTargets(1, &directXCommon->rtvHandles_[directXCommon->backBufferIndex_], false, &dsvHandle);
		//指定した深度で画面全体をクリアする
		directXCommon->commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		//描画!(DrawCall/ドローコール)。
		directXCommon->commandList_->DrawIndexedInstanced(UINT(modelData.vertices.size()), 1, 0, 0, 0);

		//スプライトの描画するコマンドを積む
		//使用するテクスチャの決定
		//マテリアルCBufferの場所を設定
		directXCommon->commandList_->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());//material
		directXCommon->commandList_->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
		directXCommon->commandList_->IASetVertexBuffers(0, 1, &vertexBufferViewSprite);//VBVを設定
		directXCommon->commandList_->IASetIndexBuffer(&indexBufferViewSprite);//IBVを設定
		//TransformMatrixCBufferの場所を設定
		directXCommon->commandList_->SetGraphicsRootConstantBufferView(1, transformationMatrixResoruceSprite->GetGPUVirtualAddress());//wvp
		//directXCommon->commandList_->DrawInstanced(6, 1, 0, 0);
		//directXCommon->commandList_->DrawIndexedInstanced(6, 1, 0, 0, 0);


		//実際のcommandListのImGuiの描画コマンドを積む
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXCommon->commandList_.Get());
		//描画の終了
		directXCommon->DrawEndFrame();
	}
	//ImGuiの終了処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	CoUninitialize();
	return 0;
}