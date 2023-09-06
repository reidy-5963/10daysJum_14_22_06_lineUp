#include "BaseCharacter.h"
#include "Scroll.h"

    /// <summary>
/// 初期化処理
/// </summary>
void BaseCharacter::Initialize() {}

/// <summary>
/// 更新処理
/// </summary>
void BaseCharacter::Update() { 
	Scroll* scroll = Scroll::GetInstance();

	sprite_->SetPosition(pos_ - scroll->GetAddScroll());
}

/// <summary>
/// 描画処理
/// </summary>
void BaseCharacter::Draw() { 
	// スプライトの描画
	sprite_->Draw();
}

void BaseCharacter::OnCollision() {}
