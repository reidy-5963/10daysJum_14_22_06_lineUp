#pragma once
#include <list>
#include "Particle.h"
#include <time.h>

class ParticleManager {
public:
	enum ParticlePattarn {
		Straight,
		Collapse,
	};

public:
	void Initialize(uint32_t tex);

	void Update();

	void ParticleUpdate();

	void Draw();

	void SetLenge(Vector2 lengePos, Vector2 radius) {
		lengePos_ = lengePos;
		radius_ = radius;
	}
	void SetTime(int settime) { 
		if (setGeneTIme_ != settime) {
			setGeneTIme_ = settime;
			generateTimer_ = 0;
		}
	}
	void SetVelo(Vector2 setvelo) { velosity_ = setvelo;
	};
	void SetSceneVelo(Vector2 setvelo) { sceneVelo = setvelo; }

	void AddParticle(Vector2 particlePos);
	void SetTecture(uint32_t texture) { tex_ = texture; }
	void SetIsParticle(bool isParticle_) { isParticle = isParticle_; }

	void SetPattern(int pattern) { pattern_ = pattern; }

	void SetEndTimer(int timer) {
		if (setTimer_ != timer) {
			setTimer_ = timer;
			endParticleTimer_ = setTimer_;

		}
	}

	bool IsEnd() { return isEnd_; }

	void SetIsTimer(bool isTimer) { isTimer_ = isTimer; }
	void Reset() { 
		generateTimer_ = 0;
	}

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
	std::list<Particle*> particles_;
	Vector2 velosity_{};
	uint32_t tex_;
	Vector2 sceneVelo;

	Vector2 lengePos_{};
	Vector2 radius_{};

	int setGeneTIme_ = 60;
	int generateTimer_ = setGeneTIme_;
	bool isParticle = false;
	
	bool isTimer_ = false;
	int setTimer_ = 120;
	int endParticleTimer_ = setTimer_;
	int pattern_ = 0;

	bool isEnd_ = false;
	float alphaoffset = 0.045f;
	Vector4 color_{};
};