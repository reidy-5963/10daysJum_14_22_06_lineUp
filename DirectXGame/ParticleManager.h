#pragma once
#include <list>
#include "Particle.h"
#include <time.h>

class ParticleManager {
public:
	enum ParticlePattarn {
		Straight,

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
	void SetTime(int settime) { setGeneTIme_ = settime;
	}
	void SetVelo(Vector2 setvelo) { velosity_ = setvelo;
	};
	void SetSceneVelo(Vector2 setvelo) { sceneVelo = setvelo; }

	void AddParticle(Vector2 particlePos);
	void SetTecture(uint32_t texture) { tex_ = texture; }
	void SetIsParticle(bool isParticle_) { isParticle = isParticle_; }
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

	int pattern = 0;
};