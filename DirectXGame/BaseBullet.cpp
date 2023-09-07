#include "BaseBullet.h"
#include <cassert>
#include "Scroll.h"

void BaseBullet::Initialize(uint32_t texture, const Vector2& pos, Vector2 velosity) {
	assert(texture != 0u);
	texture_ = texture;
	pos_ = pos;
	velocity_ = velosity;
}

void BaseBullet::Update() {
	Scroll* scroll = Scroll::GetInstance();

	sprite_->SetPosition(pos_ - scroll->GetAddScroll());
}

void BaseBullet::Draw() {}
void BaseBullet::OnCollision() {}