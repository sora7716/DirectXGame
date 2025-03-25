#include "Framework.h"

//初期化
void Framework::Initialize(){
}

//更新
void Framework::Update(){
}

//終了
void Framework::Finalize(){
}

//終了リクエスト
bool Framework::isEndRequest(){
	return WinApi::GetInstance()->ProcesMessage();
}
