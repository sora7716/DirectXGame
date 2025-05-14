#include "SRVManager.h"
#include "DirectXBase.h"
#include <cassert>
using namespace Microsoft::WRL;

//インスタンスのゲッター
SRVManager* SRVManager::GetInstance(){
	assert(!isFinalize && "GetInstance() called after Finalize()");
	if (instance == nullptr) {
		instance = new SRVManager();
	}
	return instance;
}

//初期化
void SRVManager::Initialize(DirectXBase* directXBase) {
	//DirectXの基盤部分を記憶する
	directXBase_ = directXBase;
	//デスクリプタヒープの生成
	descriptorHeap_ = directXBase_->MakeDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount, true);
	//デスクリプタ1個分のサイズを取得
	descriptorSize_ = directXBase_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

//確保
uint32_t SRVManager::Allocate() {
	//空きリストがあるなら取得
	if (!freeList_.empty()) {
		uint32_t index = freeList_.front();
		freeList_.pop();
		return index;
	}
	//上限チェック
	assert(useIndex_ < kMaxSRVCount);
	//新しい番号を割り当て
	return useIndex_++;
}

//解放
void SRVManager::Free(uint32_t index){
	//範囲内のインデックスのみ会法
	if (index >= 0 && index < useIndex_) {
		freeList_.push(index);
	}
}

// SRV生成(テクスチャ用)
void SRVManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* resource, DXGI_FORMAT format, UINT mipLevels) {
	//SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(mipLevels);
	//設定を元にSRVを生成
	directXBase_->GetDevice()->CreateShaderResourceView(resource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

// SRV生成(Structured Buffer用)
void SRVManager::CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* resource, UINT numElements, UINT structureByteStride) {
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = numElements;
	instancingSrvDesc.Buffer.StructureByteStride = structureByteStride;
	directXBase_->GetDevice()->CreateShaderResourceView(resource, &instancingSrvDesc, GetCPUDescriptorHandle(srvIndex));

}

//描画開始位置
void SRVManager::PreDraw() {
	//描画用のDescriptorHeapの設定
	ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { descriptorHeap_.Get() };
	directXBase_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());
}

//終了
void SRVManager::Finalize(){
	delete instance;
	instance = nullptr;
	isFinalize = true;
}

// rootDescriptorTableのセッター
void SRVManager::SetGrahicsRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex) {
	directXBase_->GetCommandList()->SetGraphicsRootDescriptorTable(rootParameterIndex, GetGPUDescriptorHandle(srvIndex));
}

// 最大テクスチャを超えて読み込もうとしてるかチェック
bool SRVManager::TextureLimitCheck(uint32_t kSRVTop) {
	return useIndex_+kSRVTop < kMaxSRVCount;
}

// CPUデスクリプタハンドルのゲッター
D3D12_CPU_DESCRIPTOR_HANDLE SRVManager::GetCPUDescriptorHandle(uint32_t index) {
	return directXBase_->GetCPUDescriptorHandle(descriptorHeap_, descriptorSize_, index);
}

// GPUデスクリプタハンドルのゲッター
D3D12_GPU_DESCRIPTOR_HANDLE SRVManager::GetGPUDescriptorHandle(uint32_t index) {
	return directXBase_->GetGPUDescriptorHandle(descriptorHeap_, descriptorSize_, index);
}

// デスクリプタヒープのゲッター
ID3D12DescriptorHeap* SRVManager::GetDescriptorHeap() const{
	return descriptorHeap_.Get();
}
