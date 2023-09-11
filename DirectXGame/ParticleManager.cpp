#include "ParticleManager.h"

void ParticleManager::Initialize(uint32_t tex) { tex_ = tex; }

void ParticleManager::Update() { 
	if (!isParticle) {
		generateTimer_ = 0;

	} 
	else if (isParticle) {

		if (--generateTimer_ < 0) {
			generateTimer_ = setGeneTIme_;
			Vector2 pos;
			pos.x = float(rand() % (int(radius_.x) * 2 - int(radius_.x)) + 1) + lengePos_.x;
			pos.y = float(rand() % (int(radius_.y) * 2 - int(radius_.y)) + 1) + lengePos_.y;

			

			AddParticle(pos);
		}
		ParticleUpdate();
	}

	

}

void ParticleManager::ParticleUpdate() {
	for (Particle* particle : particles_) {
		particle->Update();
	}
	// 弾の消去
	particles_.remove_if([](Particle* particle) {
		if (particle->IsDead()) {
			delete particle;
			return true;
		}
		return false;
	});
}

void ParticleManager::Draw() {
	for (Particle* particle : particles_) {
		particle->Draw();
	}
}

void ParticleManager::AddParticle(Vector2 particlePos) { 
	Particle* newParticle = new Particle;

	newParticle->Initialize(tex_, particlePos, velosity_, sceneVelo);
	particles_.push_back(newParticle);
}
