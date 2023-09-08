#include "BossBullet.h"
#include "MyMath.h"
#include <cmath>
#include <cassert>

void BossBullet::Initialize(uint32_t texture, const Vector2& pos, Vector2 velosity) 
{
	assert(texture != 0u);
	// 初期化
	texture_ = texture;
	pos_ = pos;
	velocity_ = velosity;
	bulletSpeed_ = 8.0f;

	// スプライトの生成
	sprite_.reset(Sprite::Create(texture, pos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// サイズ設定
	sprite_->SetSize({32.0f, 32.0f});
	radius_ = sprite_->GetSize().x / 2;
}

void BossBullet::Update() 
{
	pos_.x += velocity_.x * bulletSpeed_;
	pos_.y += velocity_.y * bulletSpeed_;

	BaseBullet::Update();
}

void BossBullet::FunnelInitialize(int type, Vector2& startPos, Vector2& endPos) 
{
	// イージング用の座標
	prevPos_ = startPos;
	endPos_ = endPos;
	move_t_ = 0;

	// 初手の進む方向
	moveType_ = type;

	isMove_ = true;

}

void BossBullet::FunnelUpdate() {
	
	
	BaseBullet::Update();
}

void BossBullet::Draw() 
{ 
	sprite_->Draw();
}

void BossBullet::OnCollision() 
{ 
	isDead_ = true; }

Vector2 BossBullet::BulletDirection(Vector2& BossPos, Vector2& PlayerPos) {
	Vector2 directVect = {};

	directVect = MyMath::Normalize(PlayerPos - BossPos);

	return directVect;
}

void BossBullet::BulletDirectRotate() 
{
	//// 実際に加算する値
	//Vector2 result;
	//// 進行方向をもとに回転行列の生成
	//Matrix3x3 rotateMat = MyMath::MakeRotateMatrix(std::atan2(direction_.y, direction_.x));
	//// 実際に動く値で平行移動行列を生成
	//Matrix3x3 moveMat = MyMath::MakeTranslateMatrix(move);
	//// 回転行列と平行移動行列を合成
	//moveMat = MyMath::Multiply(moveMat, rotateMat);
	//// 合成した行列から移動成分のみ取得
	//result = {moveMat.m[2][0], moveMat.m[2][1]};

	//return result;
}
