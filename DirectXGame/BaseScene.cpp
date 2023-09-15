#include "BaseScene.h"

int BaseScene::sceneNum = TITLE;

bool BaseScene::isBossDead = false;


void BaseScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
}
