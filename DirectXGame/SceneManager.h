#pragma once
#include <memory>
#include "BaseScene.h"
#include "TitleScene.h"
#include "GameScene.h"

class SceneManager 
{
public:
	SceneManager();
	~SceneManager();

	void Update();

	void Draw();

private:
	std::unique_ptr<BaseScene> sceneArray_[3];

	int sceneNum_;
	int prevSceneNum_;

};
