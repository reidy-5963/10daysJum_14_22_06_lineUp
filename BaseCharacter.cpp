#include "BaseCharacter.h"

/// <summary>
/// 初期化処理
/// </summary>
void BaseCharacter::Initialize() {}

/// <summary>
/// 更新処理
/// </summary>
void BaseCharacter::Update() {}

/// <summary>
/// 描画処理
/// </summary>
void BaseCharacter::Draw() { 
	// スプライトの描画
	sprite_->Draw();
}
