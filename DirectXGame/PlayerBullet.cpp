#include "PlayerBullet.h"
#include <cassert>
#include "GlobalVariables.h"

void PlayerBullet::Initialize(uint32_t texture, const Vector2& pos, Vector2 velosity) {
	assert(texture != 0u);
	texture_ = texture;
	pos_ = pos;
	direction_ = velosity;
	radius_ = 8.0f;

	// スプライトの生成
	sprite_.reset(Sprite::Create(texture, pos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
}

void PlayerBullet::Update() {

	pos_.x += direction_.x * bulletSpeed;
	pos_.y += direction_.y * bulletSpeed;

	sprite_->SetSize({radius_ * 2, radius_ * 2});
	BaseBullet::Update();
}

void PlayerBullet::Draw() {

	//
	sprite_->Draw();
}

void PlayerBullet::OnCollision() { isDead_ = true; }
