#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(uint32_t texture) {
	assert(texture != 0u);
	
	// スプライトの生成
	sprite_.reset(Sprite::Create(texture, {720, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	
}

void PlayerBullet::Update() { 
	//
	sprite_->SetPosition(pos_);
}

void PlayerBullet::Draw() { 
	
	//
	sprite_->Draw();
}
