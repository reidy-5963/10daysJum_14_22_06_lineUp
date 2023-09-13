#include "Enemy.h"
#include "Animation.h"
#include "ImGuiManager.h"
#include "Sprite.h"
#include "TextureManager.h"
#include <numbers>
#include "Audio.h"

void Enemy::Initialize() {
	// 変数系初期化
	pos_ = {300, 300};
	radius_ = 48;
	velocity_ = {};

	// スプライトの初期化
	sprite_.reset(
	    Sprite::Create(charaTex_, {pos_.x, pos_.y}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	sprite_->SetSize({radius_ * 2, radius_ * 2});

	// アニメーションの変数初期化
	AnimationValueInitialize();

	// パーティクルの初期化
	particle_ = std::make_unique<ParticleManager>();
	particle_->Initialize(particleTex_);
}

void Enemy::AnimationValueInitialize() {
	animationTimer = 0;
	animationNumber = 0;
	animationScene = 3;
	oneTime = 4;
	isAnimation = true;
}

void Enemy::Update() {
	ScreenPosInitialize();
#ifdef _DEBUG
	ImGui::Begin("EnemyInfo");
	ImGui::DragFloat2("pos", &pos_.x, 0.1f, 0, 2000.0f);
	ImGui::SliderFloat2("speed", &velocity_.x, 0.0f, 30.0f);
	ImGui::End();

#endif // _DEBUG
	if (isBossParasite_) {
		isParasite_ = true;
	}
	if (!isParasite_) {
		RootStateUpdate();
	} else if (isParasite_) {
		ParasiteStateUpdate();
	}

	if (isCollapse_) {
		sprite_->SetTextureHandle(collapseTex_);
		Animation::Anime(collapseAniTimer, animationNumber, collapseAniScene, collapseAnioneTime);
		if (collapseAniTimer >= collapseAnioneTime * collapseAniScene) {
			isDead_ = true;
		}
	}

	particle_->Update();
	// 座標設定
	BaseCharacter::Update();
}

void Enemy::SetParticle() {
	particle_->SetIsParticle(true);
	particle_->SetTecture(particleTex_);
	particle_->SetLenge(pos_, {radius_, radius_});
	particle_->SetSceneVelo(sceneVelo);
	particle_->SetColor(color_);
	particle_->SetTime(17);
	particle_->SetVelo({0.0f, -5.0f});
}

void Enemy::Draw() {
	particle_->Draw();
	// 敵の描画
	BaseCharacter::Draw();
}

void Enemy::OnCollision() {
	float volume = 0.1f;
	if (!isParasite_) {
		ParasiteAnimationInitialize();
		isParasite_ = true;
		Audio::GetInstance()->PlayWave(deadSEHandle_, false, volume);
	}
}

void Enemy::ParasiteStateUpdate() { 
	// パーティクルの設定
	SetParticle();

	if (!isPossiblePickUp) {
		sprite_->SetTextureHandle(parasiteTex_);
		Animation::Anime(animationTimer, animationNumber, animationScene, oneTime);

		if (animationTimer == animationScene * oneTime) {
			isPossiblePickUp = true;
		}
	}

	if (isPossiblePickUp && !isCollapse_) {
		if (isBossParasite_) {
			isCollapse_ = true;
		}

		if (--deleteTimer < 0) {
			isCollapse_ = true;
		}
	}


}

void Enemy::RootStateUpdate() { 
	// 座標移動
	pos_ += velocity_;
	Vector2 normalize = MyMath::Normalize(velocity_);
	sprite_->SetRotation(std::atan2(normalize.y, normalize.x));

	if (pos_.x < kMinusLimits.x || pos_.x > kPlusLimits.x || pos_.y < kMinusLimits.y ||
	    pos_.y > kPlusLimits.y) {
		isDead_ = true;
	}
	Animation::Anime(animationTimer, animationNumber, animationScene, oneTime);
}

void Enemy::P2EOnCollision() { isDead_ = true; }

void Enemy::ParasiteAnimationInitialize() {
	animationTimer = 0;
	animationNumber = 0;
	animationScene = 6;
	oneTime = 4;
}

void Enemy::SetRotation(float radian) 
{ sprite_->SetRotation(radian); }
