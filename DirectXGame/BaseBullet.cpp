#include "BaseBullet.h"
#include <cassert>

void BaseBullet::Initialize(uint32_t texture, const Vector2& pos, Vector2 velosity) {
	assert(texture != 0u);
	texture_ = texture;
	pos_ = pos;
	velocity_ = velosity;
}

void BaseBullet::Update() {}

void BaseBullet::Draw() {}
// クラスの枠つくっただけやね
// さっき触れ始めた
//うん☆