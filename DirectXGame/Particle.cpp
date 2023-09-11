#include "Particle.h"
#include <cassert>
#include "Scroll.h"
#include "TextureManager.h"
#include "Animation.h"

void Particle::Initialize(uint32_t texture, Vector2 pos, Vector2 velo, Vector2 scene) {
	assert(texture != 0u);
	texture_ = texture;
	pos_ = pos;
	velosity_ = velo;
	sceneVelo = scene;
	isAnimation = true;

	float radius = 8.0f;
	sprite_.reset(
	    Sprite::Create(texture_, {pos_.x, pos_.y}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	sprite_->SetSize({radius * 2, radius * 2});
	color_ = {1.0f, 1.0f, 1.0f, 1.0f};
}

void Particle::Update() { 
	if (color_.w <= 0.0f) {
		isDead_ = true;
	}

	Animation::Anime(animationTimer, animationNumber, animationScene, oneTime);
	pos_ += velosity_;

	sprite_->SetRotation(std::atan2(velosity_.y, velosity_.x));
	// スクロールのインスタンス取得
	Scroll* scroll = Scroll::GetInstance();
	color_.w -= alphaoffset;
	// スプライトの位置を設定
	sprite_->SetPosition(pos_ - scroll->GetAddScroll() + sceneVelo);
	sprite_->SetColor(color_);
}

void Particle::Draw() {
	Scroll* scroll = Scroll::GetInstance();
	if (pos_.x < 0 + scroll->GetAddScroll().x || pos_.x > 1920 + scroll->GetAddScroll().x ||
	    pos_.y < 0 + scroll->GetAddScroll().y || pos_.y > 1080 + scroll->GetAddScroll().y) {
	} else {
		if (!isAnimation) {
			sprite_->Draw();
		} else if (isAnimation) {
			Animation::DrawAnimation(sprite_.get(), pos_, animationNumber, texture_);
		}

	}
}
