#include "BaseCharacter.h"
#include "Scroll.h"
#include "WinApp.h"
#include "TextureManager.h"
#include "Animation.h"

/// <summary>
/// 初期化処理
/// </summary>
void BaseCharacter::Initialize() {}

/// <summary>
/// 更新処理
/// </summary>
void BaseCharacter::Update() { 
	// スクロールのインスタンス取得
	Scroll* scroll = Scroll::GetInstance();

	ScreenPos += pos_ - scroll->GetAddScroll() + sceneVelo;

	// スプライトの位置の更新処理
	sprite_->SetPosition(ScreenPos);
}

/// <summary>
/// 描画処理
/// </summary>
void BaseCharacter::Draw() {
	Scroll* scroll = Scroll::GetInstance();
	if (pos_.x < 0 + scroll->GetAddScroll().x || pos_.x > 1920 + scroll->GetAddScroll().x ||
	    pos_.y < 0 + scroll->GetAddScroll().y || pos_.y > 1080 + scroll->GetAddScroll().y) {
	} else {
		// スプライトの描画
		if (!isAnimation) {
			sprite_->Draw();
		} else if (isAnimation) {
			Animation::DrawAnimation(sprite_.get(), pos_, animationNumber, charaTex_);
		}
	}
}

/// <summary>
/// 衝突処理
/// </summary>
void BaseCharacter::OnCollision() {}

void BaseCharacter::InitializeGrobalVariables() {}

void BaseCharacter::ApplyGrobalVariables() {}

