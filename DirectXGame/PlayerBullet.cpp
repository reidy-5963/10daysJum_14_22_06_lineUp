#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(uint32_t texture, const Vector2& pos, Vector2 velosity) {
	assert(texture != 0u);
	texture_ = texture;
	pos_ = pos;
	velocity_ = velosity;
		
	// スプライトの生成
	sprite_.reset(Sprite::Create(texture, pos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	//sprite_->SetSize({0.5f, 0.5f});
}

void PlayerBullet::Update() { 
	
	pos_.x += velocity_.x * BulletSpeed;
	pos_.y += velocity_.y * BulletSpeed;

	sprite_->SetSize({16.0f, 16.0f});

	sprite_->SetPosition(pos_);
}

void PlayerBullet::Draw() { 
	
	//
	sprite_->Draw();
}

void PlayerBullet::OnCollision() 
{ 
	isDead_ = true;
}
