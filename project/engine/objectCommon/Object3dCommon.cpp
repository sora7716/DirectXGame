#include "Object3dCommon.h"
#include "engine/base/DirectXBase.h"
#include "engine/3d/Camera.h"
#include <cassert>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")
using namespace Microsoft::WRL;

// デフォルトカメラのセッター
void Object3dCommon::SetDefaultCamera(Camera* camera){
	defaultCamera_ = camera;
}

// デフォルトカメラのゲッター
Camera* Object3dCommon::GetDefaultCamera() const{
	return defaultCamera_;
}
