#include "BaseCharacter.h"
#include "Scroll.h"
#include "WinApp.h"
    /// <summary>
/// 初期化処理
/// </summary>
void BaseCharacter::Initialize() {}

/// <summary>
/// 更新処理
/// </summary>
void BaseCharacter::Update() { 
	Scroll* scroll = Scroll::GetInstance();

	//if (scroll->IsScreenOut() == Scroll::UP) {
	//	pos_.y -= WinApp::kWindowHeight;
	//	scroll->SetIsScreenOut(Scroll::UNKNOWN);
	//} else if (scroll->IsScreenOut() == Scroll::DOWN) {
	//	pos_.y += WinApp::kWindowHeight;
	//	scroll->SetIsScreenOut(Scroll::UNKNOWN);
	//} else if (scroll->IsScreenOut() == Scroll::LEFT) {
	//	pos_.x -= WinApp::kWindowWidth;
	//	scroll->SetIsScreenOut(Scroll::UNKNOWN);
	//} else if (scroll->IsScreenOut() == Scroll::RIGHT) {
	//	pos_.x += WinApp::kWindowWidth;
	//	scroll->SetIsScreenOut(Scroll::UNKNOWN);
	//}


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
