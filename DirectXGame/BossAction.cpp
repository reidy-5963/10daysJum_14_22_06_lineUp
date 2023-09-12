#include "BossEnemy.h"

#include <cmath>
#include <numbers>

void BossEnemy::RushAttack() {
	// 補間レート処理
	if (rushMove_t_ >= 1.0f) {
		rushMove_t_ = 1.0f;
		isRushNow_ = false;
		behaviorRequest_ = Behavior::kRoot;
	} else {
		rushMove_t_ += 0.02f;
	}
	// 座標移動ー線形補間
	float distance = MyMath::Length(prevPlayerPos_ - prevBossPos_);
	if (distance >= easeInRange_) {
		pos_ = MyMath::EaseOutQuadF(rushMove_t_, prevBossPos_, prevPlayerPos_);
	} else {
		pos_ = MyMath::EaseInQuadF(rushMove_t_, prevBossPos_, prevPlayerPos_);
	}
	particle_->SetAlphaOffset(alphaOffset);
	particle_->SetColor(color_);
	particle_->SetTecture(particleTex);
	particle_->SetLenge(pos_, {radius_, radius_});
	particle_->SetSceneVelo(sceneVelo);
	particle_->SetTime(3);
	particle_->SetVelo({0.0f, 0.0f});
	particle_->SetPattern(ParticleManager::ParticlePattarn::Straight);
	particle_->SetIsParticle(true);

}

void BossEnemy::RushAttackInitialize() {
	// 補間レート初期化
	this->rushMove_t_ = 0;
	isRushNow_ = true;
	isRush_ = false;
}

void BossEnemy::RushAttackSetup() {
	isRush_ = true;
	// 座標初期化
	prevBossPos_ = pos_;
	prevPlayerPos_ = nowPlayerPos_;
}

void BossEnemy::GuidedAttack() {
	modeCount_ += 1;
	if (modeCount_ == kModeEndTimer_) {
		behaviorRequest_ = Behavior::kRoot;
	}

	if (modeCount_ % 30 == 0) {
		Vector2 velocity = MyMath::Normalize(nowPlayerPos_ - pos_);
		GenerateBullet(velocity, 8.0f);
	}
}

void BossEnemy::GuidedAttackInitialize() { kModeEndTimer_ = ConvertSeconds(5); }

void BossEnemy::BarrageAttack() {
	modeCount_ += 1;
	float bulletSpeed = 10.0f;
	int barrageInterval = 20;
	if (modeCount_ == kModeEndTimer_) {
		behaviorRequest_ = Behavior::kRoot;
	}
	if (modeCount_ % barrageInterval == 0) {
		// 右
		float radian = rotateDegree * ((float)std::numbers::pi / 180.0f);
		// 角度に合わせて正規化
		Vector2 norm = {std::cosf(radian), -std::sinf(radian)};
		norm = MyMath::Normalize(norm);
		GenerateBullet(norm, bulletSpeed);

		// 上
		radian = (rotateDegree + 90.0f) * ((float)std::numbers::pi / 180.0f);
		// 角度に合わせて正規化
		norm = {std::cosf(radian), -std::sinf(radian)};
		norm = MyMath::Normalize(norm);
		GenerateBullet(norm, bulletSpeed);

		// 左
		radian = (rotateDegree + 180.0f) * ((float)std::numbers::pi / 180.0f);
		// 角度に合わせて正規化
		norm = {std::cosf(radian), -std::sinf(radian)};
		norm = MyMath::Normalize(norm);
		GenerateBullet(norm, bulletSpeed);

		// 下
		radian = (rotateDegree + 270.0f) * ((float)std::numbers::pi / 180.0f);
		// 角度に合わせて正規化
		norm = {std::cosf(radian), -std::sinf(radian)};
		norm = MyMath::Normalize(norm);
		GenerateBullet(norm, bulletSpeed);
		rotateRadian_ = rotateDegree * ((float)std::numbers::pi / 180.0f);
		// 回転
		rotateDegree += 10.0f;
		rotate_t_ = 0;
	}
}

void BossEnemy::BarrageAttackInitialize() {

	kModeEndTimer_ = 150;
	rotateDegree = 180.0f / float(std::numbers::pi) * sprite_->GetRotation();
}

void BossEnemy::BeamAttack() {}

void BossEnemy::BeamAttackInitialize() {}

void BossEnemy::FunnelAttack() {
	modeCount_ += 1;

	if (modeCount_ % 120 == 0) {
		GenerateFunnel(BossFunnel::kHorizontal);
		GenerateFunnel(BossFunnel::kVertical);
	}
	if (modeCount_ == kModeEndTimer_) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void BossEnemy::FunnelAttackInitialize() {
	kModeEndTimer_ = 260;
	isFunnelAttackNow_ = true;
}
