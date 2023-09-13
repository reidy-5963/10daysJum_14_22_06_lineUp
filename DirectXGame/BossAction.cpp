#include "BossEnemy.h"
#include "Audio.h"

#include <cmath>
#include <numbers>


void BossEnemy::RushAttack() {
	// 補間レート処理
	if (rushMove_t_ >= 1.0f) {
		rushMove_t_ = 1.0f;
		isRushNow_ = false;
		behaviorRequest_ = Behavior::kRoot;
		actions_.pop_back();
		//if (actions_.back() == Behavior::kRoot) {
		//	isLastAction_ = true;
		//	Audio::GetInstance()->PlayWave(rushSpawnSEHandle_, false, 0.2f);
		//} else {
		//	isLastAction_ = false;
		//}
	} else {
		rushMove_t_ += 0.02f;
		float volume = 0.3f;
		if (!isRushSound_ && rushMove_t_ >= 0.2f) {
			isRushSound_ = true;
			Audio::GetInstance()->PlayWave(rushSEHandle_, false, volume);
		}
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
	isRushSound_ = false;
}

void BossEnemy::RushAlert() 
{ 
	#pragma region 突進の警告から開始まで
	/// 突進までの処理
	if (isRush_) {
		rushCount_ += 1;
		if (rushCount_ > kRushTimer_) {
			// 突進カウント・フラグ初期化
			rushCount_ = 0;
			isRush_ = false;
			// リクエスト
			behaviorRequest_ = Behavior::kRush;
		}
	}
#pragma endregion
}

void BossEnemy::RushAlertInitialize() 
{
	sprite_->SetTextureHandle(bossRushTex_);
	RushAttackSetup();
	float volume = 0.2f;
	Audio::GetInstance()->PlayWave(rushAlertSEHandle_, false, volume);
}

void BossEnemy::GuidedAttack() {
	modeCount_ += 1;	
	int GuidedInterval = 40; 
	if (modeCount_ == kModeEndTimer_) {
		behaviorRequest_ = Behavior::kRoot;
		actions_.pop_back();
	}

	if (modeCount_ % GuidedInterval == 0) {
		Vector2 velocity = MyMath::Normalize(nowPlayerPos_ - pos_);
		GenerateBullet(velocity, 5.0f);
	}
}

void BossEnemy::GuidedAttackInitialize() 
{ 
	sprite_->SetTextureHandle(charaTex_);

	kModeEndTimer_ = ConvertSeconds(5);
}

void BossEnemy::BarrageAttack() {
	modeCount_ += 1;
	float bulletSpeed = 10.0f;
	int barrageInterval = 25;
	if (modeCount_ == kModeEndTimer_) {
		behaviorRequest_ = Behavior::kRoot;
		actions_.pop_back();
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
	sprite_->SetTextureHandle(charaTex_);
	kModeEndTimer_ = 150;
	rotateDegree = 180.0f / float(std::numbers::pi) * sprite_->GetRotation();
}

void BossEnemy::FunnelAttack() {
	modeCount_ += 1;
	int kMaxFunnel = 2;
	if (modeCount_ % 120 == 0 && funnelCount_ < kMaxFunnel) {
		funnelCount_++;
		GenerateFunnel(BossFunnel::kHorizontal);
		GenerateFunnel(BossFunnel::kVertical);
		//Audio::GetInstance()->PlayWave(funnelSEHandle_, false, 0.3f);
	}
	if (modeCount_ >= kModeEndTimer_) {
		if (funnels_.empty() && isFunnelAttackNow_) {
			isFunnelAttackNow_ = false;
			behaviorRequest_ = Behavior::kRoot;
			actions_.pop_back();
			funnelCount_ = 0;
		} 
	}
}

void BossEnemy::FunnelAttackInitialize() {
	kModeEndTimer_ = 260;
	prevBossPos_ = pos_;
	isFunnelAttackNow_ = true;
	sprite_->SetTextureHandle(bossFunnelTex_);
}

void BossEnemy::CrossAttack()
{ 
	float firstTime = 40;
	float secondTime = 100;
	modeCount_ += 1;
	float speed = 12.5f;
	if (modeCount_ == firstTime) {
		Vector2 direct = {1.0f, 1.0f};
		direct = MyMath::Normalize(direct);
		GenerateBullet(direct, speed);

		direct = {1.0f, -1.0f};
		direct = MyMath::Normalize(direct);
		GenerateBullet(direct, speed);

		direct = {-1.0f, 1.0f};
		direct = MyMath::Normalize(direct);
		GenerateBullet(direct, speed);

		direct = {-1.0f, -1.0f};
		direct = MyMath::Normalize(direct);
		GenerateBullet(direct, speed);
	}
	speed = 12.5f;
	if (modeCount_ == secondTime) {
		Vector2 direct = {1.0f, 0.0f};
		GenerateBullet(direct, speed);
		direct = {-1.0f, 0.0f};
		GenerateBullet(direct, speed);
		direct = {0.0f, 1.0f};
		GenerateBullet(direct, speed);
		direct = {0.0f, -1.0f};
		GenerateBullet(direct, speed);
	}
	if (modeCount_ == kModeEndTimer_) {
		behaviorRequest_ = Behavior::kRoot;
		actions_.pop_back();
		
	}
}

void BossEnemy::CrossAttackInitialize() 
{ 
	sprite_->SetTextureHandle(charaTex_);
	kModeEndTimer_ = ConvertSeconds(2);
	isCrossForm_ = true;
}

void BossEnemy::CenterMove() 
{
	// 補間レート処理
	if (rushMove_t_ >= 1.0f) {
		rushMove_t_ = 1.0f;
		isRushNow_ = false;
		behaviorRequest_ = Behavior::kRoot;
		actions_.pop_back();
		if (actions_.back() == Behavior::kRoot) {
			isLastAction_ = true;
			Audio::GetInstance()->PlayWave(rushSpawnSEHandle_, false, 0.2f);
		} else {
			isLastAction_ = false;
		}
	} else {
		rushMove_t_ += 0.02f;
		float volume = 0.3f;
		if (!isRushSound_ && rushMove_t_ >= 0.2f) {
			isRushSound_ = true;
			Audio::GetInstance()->PlayWave(rushSEHandle_, false, volume);
		}
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

void BossEnemy::CenterMoveInitialize() 
{
	// 補間レート初期化
	this->rushMove_t_ = 0;
	isRushNow_ = true;
	// 座標初期化
	prevBossPos_ = pos_;
	prevPlayerPos_ = Vector2(1920.0f, 1080.0f);
}
