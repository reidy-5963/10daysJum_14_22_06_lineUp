#include "Enemy.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "ImGuiManager.h"

void Enemy::Initialize() 
{
	pos_ = {300, 300};

	sprite_.reset(
	    Sprite::Create(charaTex_, {pos_.x, pos_.y}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	radius_ = 32.0f;

}

void Enemy::Update() 
{ 

	// 座標設定
	BaseCharacter::Update();
}

void Enemy::Draw() 
{ 
	// 敵の描画
	BaseCharacter::Draw();
}

void Enemy::OnCollision() 
{ 
	isDead_ = true;
}
