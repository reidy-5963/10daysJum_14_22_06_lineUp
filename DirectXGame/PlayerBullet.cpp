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

	pos_.x += direction_.x * bulletSpeed;
	pos_.y += direction_.y * bulletSpeed;

	sprite_->SetSize({radius_ * 2, radius_ * 2});
	BaseBullet::Update();
	particle_->SetTecture(particleTex_);
	particle_->SetLenge(pos_, {radius_, radius_});
	particle_->SetSceneVelo(sceneVelo);
	particle_->SetTime(8);
	particle_->SetVelo({0.0f, 0.0f});
	particle_->SetIsParticle(true);
}

void PlayerBullet::Draw() {
	particle_->Draw();

	//
	sprite_->Draw();
}

void PlayerBullet::OnCollision() { isDead_ = true; }
