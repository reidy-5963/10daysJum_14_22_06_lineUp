#include "PlayerBullet.h"
#include <cassert>
#include "GlobalVariables.h"

void PlayerBullet::Initialize(uint32_t texture, const Vector2& pos, Vector2 velosity) {
	assert(texture != 0u);
	// 初期化
	texture_ = texture;
	pos_ = pos;
	direction_ = velosity;
	//radius_ = 8.0f;

	// スプライトの生成
	sprite_.reset(Sprite::Create(texture, pos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	sprite_->SetSize({radius_ * 2, radius_ * 2});
	
	particle_ = std::make_unique<ParticleManager>();
	particle_->Initialize(particleTex_);
}

void PlayerBullet::Update() {
	particle_->Update();
	if (!isCollapse) {
		particle_->SetAlphaOffset(0.1f);
		pos_.x += direction_.x * bulletSpeed;
		pos_.y += direction_.y * bulletSpeed;
		sprite_->SetSize({radius_ * 2, radius_ * 2});
		particle_->SetColor(color_);
		particle_->SetTecture(particleTex_);
		particle_->SetLenge(pos_, {radius_, radius_});
		particle_->SetSceneVelo(sceneVelo);
		particle_->SetTime(4);
		particle_->SetVelo({0.0f, 0.0f});
		particle_->SetPattern(ParticleManager::ParticlePattarn::Straight);
		particle_->SetIsParticle(true);

	} else if (isCollapse) {
		color_.w -= 0.08f;
		particle_->SetAlphaOffset(0.08f);
		particle_->SetColor(color_);
		particle_->SetEndTimer(30);
		particle_->SetIsTimer(true);
		particle_->SetPattern(ParticleManager::ParticlePattarn::Collapse);
		sprite_->SetSize({radius_ * 2, radius_ * 2});
		particle_->SetTecture(particleTex_);
		particle_->SetLenge(pos_, {radius_, radius_});
		particle_->SetSceneVelo(sceneVelo);
		particle_->SetTime(1);
		particle_->SetVelo({0.0f, -4.0f});
		particle_->SetIsParticle(true);
		sprite_->SetColor(color_);
		if (particle_->IsEnd()) {
			isDead_ = true;
		}
	}

	BaseBullet::Update();
}

void PlayerBullet::Draw() {
	particle_->Draw();

	//
	sprite_->Draw();
}

void PlayerBullet::OnCollision() {
	if (!isCollapse) {
		isCollapse = true;
	}
}
