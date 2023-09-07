#include "BossEnemy.h"
#include "TextureManager.h"
#include "MyMath.h"
#include "ImGuiManager.h"

void BossEnemy::Initialize() 
{
	input_ = Input::GetInstance();

	pos_ = {1000.0f, 500.0f};

	charaTex_ = TextureManager::Load("Enemy.png");

	sprite_.reset(
	    Sprite::Create(charaTex_, {pos_.x, pos_.y}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// 適当にサイズ
	sprite_->SetSize(Vector2(300.0f, 300.0f));
	// 当たり判定用の半径（サイズに合わせる）
	radius_ = 32.0f;


}

void BossEnemy::Update() 
{
	if (input_->TriggerKey(DIK_H)) {
		rushFlag_ = true;
	}

	if (rushFlag_) {
		rushCount_ += 1;
		if (behavior_ != Behavior::kRush && rushCount_ > kRushTimer_) {
			behaviorRequest_ = Behavior::kRush;
			// 突進カウント・フラグ初期化
			rushCount_ = 0;
			rushFlag_ = false;
		}
	}

	ImGui::Begin("cc");
	ImGui::Text("count : %d\nisRush : %d", rushCount_, rushFlag_);
	ImGui::End();

	if (behaviorRequest_) {
		// 行動変更
		behavior_ = behaviorRequest_.value();

		// それぞれの初期化
		switch (behavior_) {
		case BossEnemy::Behavior::kRoot:

			break;
		// 突進初期化
		case BossEnemy::Behavior::kRush:
			RushAttackInitialize();
			break;
		// 誘導弾初期化
		case BossEnemy::Behavior::kGuided:

			break;
		// 弾幕初期化
		case BossEnemy::Behavior::kBarrage:

			break;
		// ビーム初期化
		case BossEnemy::Behavior::kBeam:
			BeamAttackInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
	case BossEnemy::Behavior::kRoot:
		
		break;
	// 突進処理
	case BossEnemy::Behavior::kRush:
		RushAttack();
		break;
	// 誘導弾処理
	case BossEnemy::Behavior::kGuided:

		break;
	// 弾幕処理
	case BossEnemy::Behavior::kBarrage:

		break;
	// ビーム処理
	case BossEnemy::Behavior::kBeam:
		BeamAttack();
		break;
	}


	// 座標設定
	BaseCharacter::Update();
}

void BossEnemy::Draw() 
{ 
	BaseCharacter::Draw();

}

void BossEnemy::OnCollision() {}

void BossEnemy::RushAttack() 
{
	// 補間レート処理
	if (rushMove_t_ >= 1.0f) {
		rushMove_t_ = 1.0f;
		behaviorRequest_ = Behavior::kRoot;
	} else {
		rushMove_t_ += 0.01f;
	}
	// 座標移動ー線形補間
	pos_ = MyMath::lerp(rushMove_t_, prevBossPos_, prevPlayerPos_);
}

void BossEnemy::RushAttackInitialize() 
{ 
	// 補間レート初期化
	this->rushMove_t_ = 0;
	// 座標初期化
	prevPlayerPos_ = nowPlayerPos_;
	prevBossPos_ = pos_;
}

void BossEnemy::BeamAttack() {}

void BossEnemy::BeamAttackInitialize() {}
