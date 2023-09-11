#include "BossFunnel.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include <cassert>
#include <cmath>
#include "Animation.h"
void BossFunnel::Initialize(uint32_t texture, int type, Vector2& startPos, Vector2& endPos) 
{
	particle_ = std::make_unique<ParticleManager>();
	particle_->Initialize(particleTex_);

	// イージング用の座標
	prevPos_ = startPos;
	endPos_ = endPos;
	move_t_ = 0;

	// 初手の進む方向
	switch (type) {
	case kHorizontal:
		moveType_ = kVertical;
		break;
	case kVertical:
		moveType_ = kHorizontal;
		break;
	}

	isMove_ = true;

	turnCount_ = 0;
	turnCoolTime_ = 0;

	// 初期化
	texture_ = texture;
	pos_ = startPos;
	animationTimer = 0;
	animationNumber = 0;
	animationScene = 6;
	oneTime = 5;
	isAnimation = true;

	// スプライトの生成
	sprite_.reset(Sprite::Create(texture, pos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// サイズ設定
	radius_ = 64.0f;
	sprite_->SetSize({radius_ * 2, radius_ * 2});
	direction_ = endPos_ - prevPos_;
	sprite_->SetRotation(std::atan2(direction_.y, direction_.x));
}

void BossFunnel::Update(Vector2& playerPos) {
#ifdef _DEBUG
	ImGui::Begin("fla");
	ImGui::Text("%d", turnCount_);
	ImGui::End();

#endif // _DEBUG

	// 線形補間での移動
	if (isMove_ && turnCount_ < 2) {
		pos_ = MyMath::EaseInCubicF(move_t_, prevPos_, endPos_);
		if (move_t_ >= 1.0f) {
			// 回数
			turnCount_ += 1;

			move_t_ = 0;
			isMove_ = false;
		} else {
			move_t_ += 0.02f;
		}
	} 
	// 直進し続ける処理
	else if (isMove_ && turnCount_ == 2) {
		kMoveSpeed_ += 5.0f * (1.0f / 60.0f);
		pos_ += Vector2(moveDirect_.x * kMoveSpeed_, moveDirect_.y * kMoveSpeed_);
		if (kMoveSpeed_ < 0.5f) {
			isDead_ = true;
		}
	}
	// 移動していないカウント
	else if (!isMove_) {
		turnCoolTime_ += 1;
	}
	// クールタイムカウント
	if (turnCoolTime_ == kTurnCoolTime) {
		turnCoolTime_ = 0;
		isMove_ = true;
		// 今の場所
		prevPos_ = pos_;
		// 次の曲がるタイプ
		switch (moveType_) {
		case kHorizontal:
			moveType_ = kVertical;
			endPos_ = {playerPos.x, pos_.y};
			moveDirect_ = MyMath::Normalize(endPos_ - pos_);
			break;
		case kVertical:
			moveType_ = kHorizontal;
			endPos_ = {pos_.x, playerPos.y};
			moveDirect_ = MyMath::Normalize(endPos_ - pos_);
			break;
		}
		// 向きの回転
		direction_ = endPos_ - prevPos_;
		//sprite_->SetRotation(std::atan2(direction_.y, direction_.x));
		if (turnCount_ == 2) {
			kMoveSpeed_ = kInitSpeed;
		}
	}
	particle_->SetAlphaOffset(0.05f);
	particle_->SetColor(color_);
	particle_->SetTecture(particleTex_);
	particle_->SetLenge(pos_, {radius_, radius_});
	particle_->SetSceneVelo(sceneVelo);
	particle_->SetTime(3);
	particle_->SetVelo({0.0f, 0.0f});
	particle_->SetPattern(ParticleManager::ParticlePattarn::Straight);
	particle_->SetIsParticle(true);

	particle_->Update();

	Animation::Anime(animationTimer, animationNumber, animationScene, oneTime);

	BaseBullet::Update();
}

void BossFunnel::Draw() {
	particle_->Draw();
	BaseBullet::Draw();
}

void BossFunnel::OnCollision() 
{ isDead_ = true; }
