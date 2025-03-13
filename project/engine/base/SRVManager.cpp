#include "SRVManager.h"
#include "DirectXBase.h"
#include <cassert>

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
	//上限に達してないか？
	assert(kMaxSRVCount < useIndex_);
	//returnする番号を一旦記録しておく
	int index = useIndex_;
	//次回のために番号を1進める
	useIndex_++;
	//上で記録した番号をreturn
	return index;
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
	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN; // Structured Buffer の場合は UNKNOWN
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER; // バッファ用の設定
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = structureByteStride;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE; // 必要なら RAW を設定
	// 設定を元に SRV を生成
	directXBase_->GetDevice()->CreateShaderResourceView(resource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

// CPUデスクリプタハンドルのゲッター
D3D12_CPU_DESCRIPTOR_HANDLE SRVManager::GetCPUDescriptorHandle(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}

// GPUデスクリプタハンドルのゲッター
D3D12_GPU_DESCRIPTOR_HANDLE SRVManager::GetGPUDescriptorHandle(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}
