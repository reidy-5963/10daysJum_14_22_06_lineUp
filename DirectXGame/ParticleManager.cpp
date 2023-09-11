#include "ParticleManager.h"

void ParticleManager::Initialize(uint32_t tex) { tex_ = tex; }

void ParticleManager::Update() { 
	if (!isParticle) {
		generateTimer_ = 0;

	} 
	else if (isParticle) {
		if (isTimer_) {
			if (--endParticleTimer_ < 0) {
				isEnd_ = true;
			}
		}
		if (--generateTimer_ < 0) {
			if (pattern_ == Straight) {
				generateTimer_ = setGeneTIme_;
				Vector2 pos;
				if (radius_.x == 0 || radius_.y == 0) {
				} else {
					pos.x = float(rand() % (int(radius_.x) * 2 - int(radius_.x)) + 1) + lengePos_.x;
					pos.y = float(rand() % (int(radius_.y) * 2 - int(radius_.y)) + 1) + lengePos_.y;
					AddParticle(pos);

				}

			} 
			else if (pattern_ == Collapse) {
				Vector2 pos;
				float rad = float(rand() % 5);
				//for (int i = 0; i < 5; i++) {
					pos = lengePos_;
					Matrix3x3 moveMat = MyMath::MakeTranslateMatrix(velosity_);
					Matrix3x3 rotateMat = MyMath::MakeRotateMatrix(rad + (3.14f * 0.25f));
					moveMat = MyMath::Multiply(moveMat, rotateMat);

					velosity_ = {moveMat.m[2][0], moveMat.m[2][1]};
					AddParticle(pos);

				//}

			}
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
	newParticle->SetColor(color_);
	newParticle->SetAlphaOffset(alphaoffset);
	particles_.push_back(newParticle);
}
