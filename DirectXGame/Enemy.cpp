#include "Enemy.h"
#include "Animation.h"
#include "ImGuiManager.h"
#include "Sprite.h"
#include "TextureManager.h"
#include <numbers>

void Enemy::Initialize() {
	pos_ = {300, 300};

	sprite_.reset(
	    Sprite::Create(charaTex_, {pos_.x, pos_.y}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	radius_ = 48;
	sprite_->SetSize({radius_ * 2, radius_ * 2});
	velocity_ = {};
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


	if (!isParasite_) {
		// 座標移動
		pos_ += velocity_;
		Vector2 normalize = MyMath::Normalize(velocity_);
		sprite_->SetRotation(std::atan2(normalize.y, normalize.x));

		if (pos_.x < kMinusLimits.x || pos_.x > kPlusLimits.x || pos_.y < kMinusLimits.y ||
		    pos_.y > kPlusLimits.y) {
			isDead_ = true;
		}
		Animation::Anime(animationTimer, animationNumber, animationScene, oneTime);

	} else if (isParasite_) {
		if (!isPopUpPlayer) {
			sprite_->SetTextureHandle(parasiteTex_);
			Animation::Anime(animationTimer, animationNumber, animationScene, oneTime);

			if (animationTimer == animationScene * oneTime) {

				isPopUpPlayer = true;
			}
		}

		if (isPopUpPlayer) {
			if (--deleteTimer < 0) {
				isDead_ = true;
			}
		}
	}

	// 座標設定
	BaseCharacter::Update();
}

void Enemy::Draw() {
	// 敵の描画
	BaseCharacter::Draw();
}

void Enemy::OnCollision() {
	if (!isParasite_) {
		ParasiteAnimationInitialize();
		isParasite_ = true;

	}
}

void Enemy::P2EOnCollision() { isDead_ = true; }

void Enemy::ParasiteAnimationInitialize() {
	animationTimer = 0;
	animationNumber = 0;
	animationScene = 6;
	oneTime = 4;
}
