#pragma once
#include "MyMath.h"
#include "Sprite.h"
#include <memory>

class Particle {
public:
	void Initialize(uint32_t texture, Vector2 pos, Vector2 velo, Vector2 scene);

	void Update();

	void Draw();
	bool IsDead() { return isDead_; }

	void SetAlphaOffset(float offset) { 
		if (alphaoffset != offset) {
			alphaoffset = offset;
		}
	}
	void SetColor(Vector4 color) {
		if (color_.w != color.w) {
			color_ = color;
		}
	}

private:
	std::unique_ptr<Sprite> sprite_ = nullptr;
	uint32_t texture_ = 0u;

	Vector2 pos_{};
	Vector2 velosity_{};

	Vector2 sceneVelo{};
	Vector4 color_{};
	bool isDead_ = false;

	float alphaoffset = 0.045f;
	int animationTimer = 0;
	int animationNumber = 0;
	int animationScene = 5;
	int oneTime = 3;
	bool isAnimation = false;

	int setTimer = 20;
	int timer = setTimer;
};