#pragma once
#include "Sprite.h"

class Animation {
public:
	static void Anime(int& count, int& number, const int scene, const int oneTime);

	static void DrawAnimation(
	    Sprite* sprite, int texX, int texY, int srcX, int srcY, int srcW, int srcH,
	    uint32_t texture);

	static void DrawAnimation(Sprite* sprite, Vector2 pos, int animationNum, uint32_t tex);

private:


};