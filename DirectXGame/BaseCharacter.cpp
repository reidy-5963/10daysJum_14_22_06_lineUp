#include "BaseCharacter.h"
#include "Scroll.h"
#include "WinApp.h"
#include "TextureManager.h"

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

	Vector2 ScreenPos = pos_ - scroll->GetAddScroll();

	// スプライトの位置の更新処理
	sprite_->SetPosition(ScreenPos);
}

/// <summary>
/// 描画処理
/// </summary>
void BaseCharacter::Draw() { 
	// スプライトの描画
	if(!isAnimation) { 
		sprite_->Draw();
	} else if (isAnimation) {
		DrawAnimation(
		    sprite_.get(), int(pos_.x), int(pos_.y),
		    int(animationNumber * (radius_ * 2)), 0, int(radius_ * 2), int(radius_ * 2), charaTex_);
	}
}

/// <summary>
/// 衝突処理
/// </summary>
void BaseCharacter::OnCollision() {}

void BaseCharacter::InitializeGrobalVariables() {}

void BaseCharacter::ApplyGrobalVariables() {}

void BaseCharacter::DrawAnimation(
    Sprite* sprite, int texX, int texY, int srcX, int srcY, int srcW, int srcH, uint32_t texture) {
	const D3D12_RESOURCE_DESC& texDesc = TextureManager::GetInstance()->GetResoureDesc(texture);
	texX;
	texY;
	if (srcX < 0 || srcY < 0 || srcW < 0 || srcH < 0) {
		sprite->SetTextureRect({0.0f, 0.0f}, {(float)texDesc.Width, (float)texDesc.Height});
	} else {
		sprite->SetTextureRect({(float)srcX, (float)srcY}, {(float)srcW, (float)srcH});
	}
	sprite->Draw();
}
