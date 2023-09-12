#pragma once

enum Scene { TITLE, GAMESCENE, CLEAR, };

class BaseScene 
{
public:

	virtual void Initialize(){};
	virtual void Update(){};
	virtual void Draw(){};

	int GetSceneNum() { return sceneNum; }

protected:
	static int sceneNum;
	
	static int resultTime;
	static int resultScore;
	static int resultKillCount;
	static bool isBossDead;
	static bool is6Tails;
};
