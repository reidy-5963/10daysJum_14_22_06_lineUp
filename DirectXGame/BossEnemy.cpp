#include "BossEnemy.h"
#include "TextureManager.h"

void BossEnemy::Initialize() 
{

	pos_ = {1000.0f, 500.0f};

	charaTex_ = TextureManager::Load("Enemy.png");

	sprite_.reset(
	    Sprite::Create(charaTex_, {pos_.x, pos_.y}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// 適当にサイズ
	sprite_->SetSize(Vector2(300.0f, 300.0f));
	// 当たり判定用の半径（サイズに合わせる）
	radius_ = 32.0f;


}

void BossEnemy::Update() 
{

	// 座標設定
	sprite_->SetPosition(pos_);
}

void BossEnemy::Draw() 
{ 
	BaseCharacter::Draw();

}

void BossEnemy::OnCollision() {}
