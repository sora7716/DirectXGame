#include "2d/TextureManager.h"
#include "math/func/Math.h"
#include "audio/Audio.h"
#include "input/Input.h"
#include "base/D3DResourceLeakChecker.h"
#include "2d/SpriteGeneral.h"
#include "2d/Sprite.h"
#include <vector>
#include <fstream>
#include <sstream>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

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

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	HRESULT hr = S_FALSE;
	D3DResourceLeakChecker leakChacker;//メモリーリークをチェック
	//ウィンドウズアプリケーション
	WinApi* winApi = WinApi::GetInstance();
	//DirectXCommon
	std::unique_ptr<DirectXBase>directXBase = std::make_unique<DirectXBase>();
	std::unique_ptr<SpriteGeneral>spriteGeneral = std::make_unique<SpriteGeneral>();
	//ウィンドウの作成
	winApi->Initialize();
	//DirectX12の初期化
	directXBase->Initialize();
	//テクスチャマネージャーの初期化
	TextureManager::GetInstance()->Initialize(directXBase.get());
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
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = directXBase->CompilerShader(L"engine/resources/shaders/Object3d.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	//PixelShader
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = directXBase->CompilerShader(L"engine/resources/shaders/object3d.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	//PSOを生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	//graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();//RootSignature
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
	graphicsPipelineStateDesc.DepthStencilState = directXBase->depthStencilDesc_;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	spriteGeneral->Initialize(directXBase.get(), graphicsPipelineStateDesc);

	//マテリアル用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = directXBase->CreateBufferResource(sizeof(Material));
	//マテリアルにデータを書き込む
	Material* materialData = nullptr;
	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//色を書き込む
	materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData->enableLighting = true;
	materialData->uvTransform = Math::MakeIdentity4x4();

	//WVP用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource = directXBase->CreateBufferResource(sizeof(TransformationMatrix));
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
	ModelData modelData = LoadObjeFile("engine/resources/cube", "cube.obj");

	//Index用(3dGameObject)
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource = directXBase->CreateBufferResource(sizeof(uint32_t) * modelData.vertices.size());
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
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = directXBase->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
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

	//光源のリソースを作成
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource = directXBase->CreateBufferResource(sizeof(DirectionalLight));
	DirectionalLight* directionalLightData = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;

	TextureManager::GetInstance()->LoadTexture("engine/resources/texture/uvChecker.png");
	//スプライト
	std::vector<Vector2> pos;
	std::vector<Sprite*>sprites;
	for (uint32_t i = 0; i < 5; i++) {
		Sprite* sprite = new Sprite();
		sprite->Initialize(spriteGeneral.get(), "engine/resources/texture/uvChecker.png");
		pos.push_back(Vector2(120.0f, 0.0f) * (float)i);
		sprite->SetPosition(pos[i]);
		sprite->SetSize({ 100.0f,180.0f });
		sprites.push_back(sprite);
	}

	//GameObjectの初期化
	//Transform変数を作成
	Transform transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1.7f,-1.0f,0.0f} };
	//テクスチャの切り替え用
	bool useMonsterBall = true;

	//カメラ
	Transform cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f,},{0.0f,0.0f,-10.0f} };
	TextureManager::GetInstance()->LoadTexture(modelData.material.textureFilePath);
	uint32_t textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData.material.textureFilePath);
	//// Textureを呼んで転送する
	//DirectX::ScratchImage mipImages = DirectXBase::LoadTexture("engine/resources/texture/uvChecker.png");
	//const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	//Microsoft::WRL::ComPtr<ID3D12Resource> textureResource = directXBase->CreateTextureResource(metadata);
	//Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = directXBase->UploadTextureData(textureResource.Get(), mipImages);
	////2枚目のTextureを読む
	//DirectX::ScratchImage mipImages2 = DirectXBase::LoadTexture(modelData.material.textureFilePath);
	//const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	//Microsoft::WRL::ComPtr<ID3D12Resource> textureResource2 = directXBase->CreateTextureResource(metadata2);
	/*Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource2 = directXBase->UploadTextureData(TextureManager::GetInstance()->GetTextureData()[textureIndex].resourece.Get(), mipImages2);*/

	////metaDataを基にSRVの設定
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	//srvDesc.Format = metadata.format;
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	//srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	////metaData2を基にSRVの設定
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	//srvDesc2.Format = metadata2.format;
	//srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D11テクスチャ
	//srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);

	////SRVを作成するDescriptorHeapの場所を決める
	//D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = DirectXBase::GetCPUDescriptorHandle(directXBase->srvDescriptorHeap_.Get(), directXBase->descriptorSizeSRV_, 1);
	//D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = DirectXBase::GetGPUDescriptorHandle(directXBase->srvDescriptorHeap_.Get(), directXBase->descriptorSizeSRV_, 1);

	////SRVの生成
	//directXBase->GetDevice()->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);

	////SRVを作成するDescriptorHeapの場所を決める
	//D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = DirectXBase::GetCPUDescriptorHandle(directXBase->srvDescriptorHeap_.Get(), directXBase->descriptorSizeSRV_, 2);
	//D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = DirectXBase::GetGPUDescriptorHandle(directXBase->srvDescriptorHeap_.Get(), directXBase->descriptorSizeSRV_, 2);
	////SRVの生成
	//directXBase->GetDevice()->CreateShaderResourceView(textureResource2.Get(), &srvDesc2, textureSrvHandleCPU2);

	DirectionalLight directionalLight;
	directionalLight.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight.direction = { 0.0f,-1.0f,0.0f };
	directionalLight.intensity = 1.0f;

	//UVTransform用の変数
	Transform uvTransform{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	//音
	Audio* audio_ = Audio::GetInstance();
	//初期化
	audio_->Initialize();
	//音声読み込み
	audio_->SetSoundData(0, "mokugyo.wav");
	//音声の再生
	audio_->SoundPlayWave(0, true);
	float volume = 1.0f;

	//入力関数の初期化処理
	//DirectInputの初期化
	Input* input = Input::GetInstance();
	input->Initialize();

	//ウィンドウの✖ボタンが押されるまでループ
	while (true) {
		if (winApi->ProcesMessage()) {
			break;
		}
		//ImGuiの開始
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		input->Update();

		if (input->PressKey(DIK_W)) {
			transform.translate.y += 0.01f;
		}
		else if (input->PressKey(DIK_S)) {
			transform.translate.y -= 0.01f;
		}
		if (input->PressKey(DIK_D)) {
			transform.translate.x += 0.01f;
		}
		else if (input->PressKey(DIK_A)) {
			transform.translate.x -= 0.01f;
		}
		//更新処理
		//transform.rotate.y += 0.001f;
		TransformationMatrix sphereWvpData;
		sphereWvpData.World = Rendering::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
		Matrix4x4 cameraMatrix = Rendering::MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
		Matrix4x4 viewMatrix = ~cameraMatrix;
		Matrix4x4 projectionMatrix = Rendering::MakePerspectiveFovMatrix(0.45f, float(WinApi::kClientWidth) / float(WinApi::kClientHeight), 0.1f, 100.0f);
		sphereWvpData.WVP = sphereWvpData.World * viewMatrix * projectionMatrix;
		*wvpData = sphereWvpData;


		//スプイライト用の更新
		for (uint32_t i = 0; i < sprites.size(); i++) {
			sprites[i]->SetPosition(pos[i]);
			sprites[i]->Update();
		}
		//ライト
		directionalLight.direction = Math::Normalize(directionalLight.direction);
		*directionalLightData = directionalLight;
		//開発用のUIの処理
		/*ImGui::ShowDemoWindow();*/
		ImGui::Begin("3dObject");
		ImGui::DragFloat3("scale", &transform.scale.x, 0.1f, 0.0f, 5.0f);
		ImGui::DragFloat3("rotate", &transform.rotate.x, 0.1f);
		ImGui::DragFloat3("translate", &transform.translate.x, 0.1f);
		ImGui::Checkbox("useMonsterBall", &useMonsterBall);
		ImGui::End();

		ImGui::Begin("light");
		ImGui::ColorEdit4("color", &directionalLight.color.x);
		ImGui::DragFloat3("direction", &directionalLight.direction.x, 0.1f);
		ImGui::DragFloat("intensity", &directionalLight.intensity, 0.1f, 0.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("UV:sphere");
		ImGui::DragFloat2("scale", &uvTransform.scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::SliderAngle("rotate", &uvTransform.rotate.z);
		ImGui::DragFloat2("translate", &uvTransform.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("sound");
		ImGui::DragFloat("volume", &volume, 0.01f, 0.0f, 2.0f);
		ImGui::End();

		ImGui::Begin("sprite");
		//ImGui::DragFloat3("scale", &size.x, 0.1f, 0.0f, 5.0f);
		//ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
		ImGui::DragFloat2("translate", &pos[0].x, 0.1f);
		ImGui::End();

		/*	ImGui::Begin("UV:sprite");
			ImGui::DragFloat2("scale", &uvTransform_.scale.x, 0.01f, -10.0f, 10.0f);
			ImGui::SliderAngle("rotate", &uvTransform_.rotate.z);
			ImGui::DragFloat2("translate", &uvTransform_.translate.x, 0.01f, -10.0f, 10.0f);
			ImGui::End();*/

		audio_->SetVolume(0, volume);

		//ImGuiの内部コマンドを生成する
		ImGui::Render();
		//描画処理
		directXBase->PreDraw();
		//描画するコマンドを積む
		//スプライトの描画準備
		spriteGeneral->DrawSetting();
		//RootSignatureを設定。PSOに設定しているけど別途設定が必要
		directXBase->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);//VBVを設定
		directXBase->GetCommandList()->IASetIndexBuffer(&indexBufferView);//IBVを設定
		//マテリアルCBufferの場所を設定
		directXBase->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
		//wvp用のCBufferの場所を設定
		directXBase->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
		//平行光源CBufferの場所を設定
		directXBase->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
		//SRVのDescriptorTableの先頭を設定	。2はrootParameter[2]のこと
		//directXBase->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureIndex));
		////描画!(DrawCall/ドローコール)。
		//directXBase->GetCommandList()->DrawIndexedInstanced(UINT(modelData.vertices.size()), 1, 0, 0, 0);

		//スプライトの描画するコマンドを積む
		for (auto sprite : sprites) {
			sprite->Draw();
		}

		//実際のcommandListのImGuiの描画コマンドを積む
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXBase->GetCommandList());
		//描画の終了
		directXBase->PostDraw();
	}
	//ImGuiの終了処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	//WindowsAPIの終了処理
	winApi->Finalize();
	//Audioの終了処理
	audio_->Finalize();
	for (auto sprite : sprites) {
		delete sprite;
	}
	sprites.clear();
	//テクスチャマネージャーの終了処理
	TextureManager::GetInstance()->Finalize();
	return 0;
}