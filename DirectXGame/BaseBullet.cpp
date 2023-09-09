#include "BaseBullet.h"
#include <cassert>
#include "Scroll.h"

/// <summary>
/// 初期化処理
/// </summary>
void BaseBullet::Initialize(uint32_t texture, const Vector2& pos, Vector2 velosity) {
	assert(texture != 0u);

	// 初期化
	texture_ = texture;
	pos_ = pos;
	direction_ = velosity;
}

/// <summary>
/// 描画処理
/// </summary>
void BaseBullet::Update() {
	// スクロールのインスタンス取得
	Scroll* scroll = Scroll::GetInstance();
	//Vector2 ScreenPos;
//	ScreenPos += ;
	
	// スプライトの位置を設定
	sprite_->SetPosition(pos_ - scroll->GetAddScroll() + sceneVelo);
}

/// <summary>
/// 描画処理
/// </summary>
void BaseBullet::Draw() { 
	// スプライトの描画処理
	sprite_->Draw();
}

/// <summary>
/// 衝突処理
/// </summary>
void BaseBullet::OnCollision() {}
void BaseBullet::SetSize(Vector2 size) 
{ sprite_->SetSize(size); }
Vector2 BaseBullet::GetSize() { return sprite_->GetSize(); }
// クラスの枠つくっただけやね
// さっき触れ始めた
//うん☆