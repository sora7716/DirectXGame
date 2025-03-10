#include "base/DirectXBase.h"
#include "2d/TextureManager.h"
#include "math/func/Math.h"
#include "audio/Audio.h"
#include "input/Input.h"
#include "base/D3DResourceLeakChecker.h"
#include "objectCommon/SpriteCommon.h"
#include "2d/Sprite.h"
#include "objectCommon/Object3dCommon.h"
#include "3d/Object3d.h"
#include "3d/ModelManager.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	HRESULT hr = S_FALSE;
	D3DResourceLeakChecker leakChacker;//メモリーリークをチェック
	//ウィンドウズアプリケーション
	WinApi* winApi = WinApi::GetInstance();
	//DirectXCommon
	std::unique_ptr<DirectXBase>directXBase = std::make_unique<DirectXBase>();
	//スプライトの共通部分
	std::unique_ptr<SpriteCommon>spriteCommon = std::make_unique<SpriteCommon>();
	//3Dオブジェクトの共通部分
	std::unique_ptr<Object3dCommon>object3dCommon = std::make_unique<Object3dCommon>();
	////モデルの共通部分
	/*std::unique_ptr<ModelCommon>modelCommon = std::make_unique<ModelCommon>();
	modelCommon->Initialize(directXBase.get());*/
	//ModelManagerの初期化
	ModelManager::GetInstance()->Initialize(directXBase.get());
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

	//スプライトの共通部分の初期化
	spriteCommon->Initialize(directXBase.get(), graphicsPipelineStateDesc);
	//3Dオブジェクトの共通部分の初期化
	object3dCommon->Initialize(directXBase.get(), graphicsPipelineStateDesc);

	//テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("engine/resources/texture/monsterBall.png");
	//スプライト
	std::vector<Vector2> pos;
	std::vector<Sprite*>sprites;
	for (uint32_t i = 0; i < 5; i++) {
		Sprite* sprite = new Sprite();
		sprite->Initialize(spriteCommon.get(), "engine/resources/texture/uvChecker.png");
		pos.push_back(Vector2(120.0f, 0.0f) * (float)i);
		sprite->SetTranslate(pos[i]);
		sprite->SetScale({ 100.0f,180.0f });
		sprites.push_back(sprite);
	}
	/*Model* model = new Model();
	model->Initialize(modelCommon.get(), "engine/resources/models", "cube", "cube.obj");*/
	ModelManager::GetInstance()->LoadModel("base", "axis.obj");
	std::vector<Transform>object3dTransforms;
	std::vector<Object3d*>object3ds;
	for (uint32_t i = 0; i < 2; i++) {
		Object3d* object3d = new Object3d();
		object3d->Initialize(object3dCommon.get());
		object3d->SetModel("axis.obj");
		Transform transform;
		transform.scale = { 1.0f,1.0f,1.0f };
		transform.rotate = {};
		transform.translate = { (float)i * -2.7f,0.0f,0.0f };
		object3dTransforms.push_back(transform);
		object3ds.push_back(object3d);
	}
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

		/*if (input->PressKey(DIK_W)) {
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
		}*/

		//スプイライト用の更新
		for (uint32_t i = 0; i < sprites.size(); i++) {
			if (i % 2 == 0) {
				sprites[i]->ChangeTexture("engine/resources/texture/uvChecker.png");
			}
			else {
				sprites[i]->ChangeTexture("engine/resources/texture/monsterBall.png");
			}
			sprites[i]->SetTranslate(pos[i]);
			sprites[i]->Update();
		}
		for (uint32_t i = 0; i < object3ds.size(); i++) {
			object3ds[i]->SetTranslate(object3dTransforms[i].translate);
			object3dTransforms[i].rotate += 0.1f;
			object3ds[i]->SetRotate(object3dTransforms[i].rotate);
			object3ds[i]->Update();
		}

		ImGui::Begin("sound");
		ImGui::DragFloat("volume", &volume, 0.01f, 0.0f, 2.0f);
		ImGui::End();

		ImGui::Begin("sprite");
		ImGui::DragFloat2("translate[0]", &pos[0].x, 0.1f);
		ImGui::DragFloat2("translate[1]", &pos[1].x, 0.1f);
		ImGui::End();

		ImGui::Begin("object3d");
		ImGui::DragFloat2("translate[0]", &object3dTransforms[0].translate.x, 0.1f);
		ImGui::DragFloat2("translate[1]", &object3dTransforms[1].translate.x, 0.1f);
		ImGui::End();
		audio_->SetVolume(0, volume);

		//ImGuiの内部コマンドを生成する
		ImGui::Render();
		//描画処理
		directXBase->PreDraw();
		//描画するコマンドを積む
		//スプライトの描画準備
		spriteCommon->DrawSetting();
		//3Dオブジェクトの描画準備
		object3dCommon->DrawSetting();

		//スプライトの描画するコマンドを積む
		for (auto sprite : sprites) {
			sprite->Draw();
		}
		//3Dオブジェクトの描画するコマンドを積む
		for (auto object3d : object3ds) {
			object3d->Draw();
		}
		//実際のcommandListのImGuiの描画コマンドを積む
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXBase->GetCommandList());
		//描画の終了
		directXBase->PostDraw();
	}
	//終了処理
	//ImGuiの終了
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	//WindowsAPIの終了
	winApi->Finalize();
	//Audioの終了
	audio_->Finalize();
	for (auto sprite : sprites) {
		delete sprite;
	}
	sprites.clear();
	for (auto object3d : object3ds) {
		delete object3d;
	}
	object3ds.clear();
	//テクスチャマネージャーの終了
	TextureManager::GetInstance()->Finalize();
	//モデルマネージャーの終了
	ModelManager::GetInstance()->Finalize();
	return 0;
}