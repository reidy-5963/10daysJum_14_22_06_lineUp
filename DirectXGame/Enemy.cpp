#include "Enemy.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "ImGuiManager.h"

void Enemy::Initialize() 
{
	pos_ = {300, 300};

	sprite_.reset(
	    Sprite::Create(charaTex_, {pos_.x, pos_.y}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	
	radius_ = 32;
	sprite_->SetSize({radius_ * 2, radius_ * 2});
	velocity_ = {};
	animationTimer = 0;
	animationNumber = 0;
	animationScene = 3;
	oneTime = 4;
	isAnimation = true;
}

void Enemy::Update() 
{
	ScreenPosInitialize();

	ImGui::Begin("EnemyInfo");
	ImGui::DragFloat2("pos", &pos_.x, 0.1f, 0, 2000.0f);
	ImGui::SliderFloat2("speed", &velocity_.x, 0.0f, 30.0f);
	ImGui::End();

	// 座標移動
	pos_ += velocity_;

	MyMath::Anime(animationTimer, animationNumber, animationScene, oneTime);

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
