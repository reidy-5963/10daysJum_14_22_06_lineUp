#include "BossEnemy.h"
#include "TextureManager.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include <numbers>
#include <cmath>
#include "BossBullet.h"
#include "WinApp.h"

BossEnemy::BossEnemy() {
	// キャラ
	charaTex_ = TextureManager::Load("Enemy.png");
	// 弾
	bulletTex_ = TextureManager::Load("Bullet.png");
	// ファンネル
	funnelTex_ = TextureManager::Load("Fannel.png");
}

void BossEnemy::RespownBoss() 
{ 
	pos_ = {float(WinApp::kWindowWidth), float(WinApp::kWindowHeight)};
}

void BossEnemy::RandomActionManager() 
{ 
	actionTimer_++;
	if (actionTimer_ == kActionCoolTime_) {
		int behaviorRand = rand() % 5 + 1;
		switch (behaviorRand) {
		case 1:
			behaviorRequest_ = Behavior::kRush;
			break;
		case 2:
			behaviorRequest_ = Behavior::kGuided;
			break;
		case 3:
			behaviorRequest_ = Behavior::kBarrage;
			break;
		case 4:
			behaviorRequest_ = Behavior::kFunnel;
			break;
		case 5:
			behaviorRequest_ = Behavior::kRush;
			break;
		}
		actionTimer_ = 0;
	}

}

void BossEnemy::GenerateBullet(Vector2& velocity) 
{
	// 生成・初期化
	BossBullet* newBullet = new BossBullet();
	newBullet->Initialize(bulletTex_, GetPosition(), velocity);
	// リストに追加
	bullets_.push_back(newBullet);
}

void BossEnemy::GenerateFunnel(int type) 
{
	// 生成初期化
	BossFunnel* newFunnel = new BossFunnel();
	Vector2 endPoint = {};
	switch (type) {
	case BossFunnel::kHorizontal:
		endPoint = {nowPlayerPos_.x, pos_.y};
		newFunnel->Initialize(funnelTex_, type, GetPosition(), endPoint);
		break;
	case BossFunnel::kVertical:
		endPoint = {pos_.x, nowPlayerPos_.y};
		newFunnel->Initialize(funnelTex_, type, GetPosition(), endPoint);
		break;
	}
	// リストに追加
	funnels_.push_back(newFunnel);
}

void BossEnemy::Initialize() 
{
	input_ = Input::GetInstance();

	pos_ = {-3000.0f, -3000.0f};

	RespownBoss();

	sprite_.reset(
	    Sprite::Create(charaTex_, {pos_.x, pos_.y}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// 適当にサイズ
	sprite_->SetSize(Vector2(300.0f, 300.0f));
	// 当たり判定用の半径（サイズに合わせる）
	radius_ = sprite_->GetSize().x / 2;

}

void BossEnemy::Update() 
{
	ScreenPosInitialize();

	ImGui::Begin("cc");
	ImGui::Text("count : %d\nisRush : %d", rushCount_, rushFlag_);
	ImGui::End();

	if (behaviorRequest_) {
		// 行動変更
		behavior_ = behaviorRequest_.value();
		// 行動状態のカウントリセット
		modeCount_ = 0;
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
			GuidedAttackInitialize();
			break;
		// 弾幕初期化
		case BossEnemy::Behavior::kBarrage:
			BarrageAttackInitialize();
			break;
		// ビーム初期化
		case BossEnemy::Behavior::kBeam:
			BeamAttackInitialize();
			break;
		case BossEnemy::Behavior::kFunnel:
			FunnelAttackInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
	/// 通常時の処理
	case BossEnemy::Behavior::kRoot:
		RootUpdate();
		break;
	/// 突進処理
	case BossEnemy::Behavior::kRush:
		RushAttack();
		break;
	/// 誘導弾処理
	case BossEnemy::Behavior::kGuided:
		GuidedAttack();
		break;
	/// 弾幕処理
	case BossEnemy::Behavior::kBarrage:
		BarrageAttack();
		break;
	/// ビーム処理
	case BossEnemy::Behavior::kBeam:
		BeamAttack();
		break;
	case BossEnemy::Behavior::kFunnel:
		FunnelAttack();
		break;
	}

	BulletUpdate();

	// 座標設定
	BaseCharacter::Update();
}

void BossEnemy::Draw() 
{ 
	for (BossBullet* bullet : bullets_) {
		bullet->Draw();
	}
	for (BossFunnel* funnel : funnels_) {
		funnel->Draw();
	}
	// 描画
	BaseCharacter::Draw();
}

void BossEnemy::OnCollision() {}

void BossEnemy::BulletUpdate() {
	bullets_.remove_if([](BossBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	for (BossBullet* bullet : bullets_) {
		bullet->SetSceneVelo(sceneVelo);
		bullet->Update();
		Vector2 size = bullet->GetSize();
		size -= Vector2(decreaseValue_, decreaseValue_);
		bullet->SetSize(size);
		bullet->SetRadius(bullet->GetSize().x / 2);
		if (bullet->GetRadius() < deadZone_) {
			bullet->SetIsDead(true);
		}
	}

	funnels_.remove_if([](BossFunnel* funnel) {
		if (funnel->GetIsDead()) {
			delete funnel;
			return true;
		}
		return false;
	});

	for (BossFunnel* funnel : funnels_) {
		funnel->SetSceneVelo(sceneVelo);
		funnel->Update(nowPlayerPos_);
		Vector2 size = funnel->GetSize();
		size -= Vector2(decreaseValue_ / 2, decreaseValue_ / 2);
		funnel->SetSize(size);
		funnel->SetRadius(funnel->GetSize().x / 2);
		if (funnel->GetRadius() < deadZone_) {
			funnel->SetIsDead(true);
		}

	}

}

void BossEnemy::RushAttack() 
{
	// 補間レート処理
	if (rushMove_t_ >= 1.0f) {
		rushMove_t_ = 1.0f;
		behaviorRequest_ = Behavior::kRoot;
	} else {
		rushMove_t_ += 0.02f;
	}
	// 座標移動ー線形補間
	float distance = MyMath::Length(prevPlayerPos_ - prevBossPos_);
	if (distance >= easeInRange_) {
		pos_ = MyMath::EaseOutQuadF(rushMove_t_, prevBossPos_, prevPlayerPos_);	
	} else {
		pos_ = MyMath::EaseInQuadF(rushMove_t_, prevBossPos_, prevPlayerPos_);
	}
}

void BossEnemy::RushAttackInitialize() 
{ 
	// 補間レート初期化
	this->rushMove_t_ = 0;
}

void BossEnemy::GuidedAttack() 
{
	modeCount_ += 1;
	if (modeCount_ == kModeEndTimer_) {
		behaviorRequest_ = Behavior::kRoot;
	}
	
	if (modeCount_ % 30 == 0) {
		Vector2 velocity = MyMath::Normalize(nowPlayerPos_ - pos_);
		GenerateBullet(velocity);
	}
}

void BossEnemy::GuidedAttackInitialize() 
{
	kModeEndTimer_ = 600;

}

void BossEnemy::BarrageAttack() 
{	
	modeCount_ += 1;
	if (modeCount_ == kModeEndTimer_) {
		behaviorRequest_ = Behavior::kRoot;
	}
	if (modeCount_ % 30 == 0) {
		// 右
		float radian = rotateDegree * ((float)std::numbers::pi / 180.0f);
		// 角度に合わせて正規化
		Vector2 norm = {std::cosf(radian), -std::sinf(radian)};
		norm = MyMath::Normalize(norm);
		GenerateBullet(norm);

		// 上
		radian = (rotateDegree + 90.0f) * ((float)std::numbers::pi / 180.0f);
		// 角度に合わせて正規化
		norm = {std::cosf(radian), -std::sinf(radian)};
		norm = MyMath::Normalize(norm);
		GenerateBullet(norm);

		// 左
		radian = (rotateDegree + 180.0f) * ((float)std::numbers::pi / 180.0f);
		// 角度に合わせて正規化
		norm = {std::cosf(radian), -std::sinf(radian)};
		norm = MyMath::Normalize(norm);
		GenerateBullet(norm);
		
		// 下
		radian = (rotateDegree + 270.0f) * ((float)std::numbers::pi / 180.0f);
		// 角度に合わせて正規化
		norm = {std::cosf(radian), -std::sinf(radian)};
		norm = MyMath::Normalize(norm);
		GenerateBullet(norm);
		rotateRadian_ = rotateDegree * ((float)std::numbers::pi / 180.0f);
		// 回転
		rotateDegree += 10.0f;
		rotate_t_ = 0;
	}
	//if (rotate_t_ >= 1.0f) {
	//	rotate_t_ += (1.0f / 30.0f);
	//	sprite_->SetRotation(MyMath::lerp(rotate_t_, sprite_->GetRotation(), rotateRadian_));
	//} else {
	//	rotate_t_ = 1.0f;
	//}
}

void BossEnemy::BarrageAttackInitialize() 
{

	kModeEndTimer_ = 150;
	rotateDegree = 180.0f / float(std::numbers::pi) * sprite_->GetRotation();

}

void BossEnemy::BeamAttack() {
	
}

void BossEnemy::BeamAttackInitialize() 
{ 

}

void BossEnemy::FunnelAttack() 
{ 
	modeCount_ += 1;

	if (modeCount_ % 120 == 0) {
		GenerateFunnel(BossFunnel::kHorizontal);
		GenerateFunnel(BossFunnel::kVertical);
	}
	if (modeCount_ == kModeEndTimer_) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void BossEnemy::FunnelAttackInitialize() 
{ kModeEndTimer_ = 260; }

void BossEnemy::RootUpdate() 
{
	/// 突進起動キー処理
	if (input_->TriggerKey(DIK_H)) {
		rushFlag_ = true;
		// 座標初期化
		prevBossPos_ = pos_;
		prevPlayerPos_ = nowPlayerPos_;
	}

	if (input_->TriggerKey(DIK_B)) {
		if (behavior_ != Behavior::kGuided) {
			behaviorRequest_ = Behavior::kGuided;
		}
	}

	if (input_->TriggerKey(DIK_K)) {
		if (behavior_ != Behavior::kBarrage) {
			behaviorRequest_ = Behavior::kBarrage;
		}
	}
	
	if (input_->TriggerKey(DIK_G)) {
		if (behavior_ != Behavior::kFunnel) {
			behaviorRequest_ = Behavior::kFunnel;
		}
	}

	if (input_->TriggerKey(DIK_L)) {
		Vector2 directRotate = nowPlayerPos_ - pos_;
		//directRotate += pos_;
		sprite_->SetRotation(std::atan2(directRotate.y, directRotate.x));
	}

	RandomActionManager();

	/// 突進までの処理
	if (rushFlag_) {
		rushCount_ += 1;
		if (behavior_ != Behavior::kRush && rushCount_ > kRushTimer_) {
			behaviorRequest_ = Behavior::kRush;
			// 突進カウント・フラグ初期化
			rushCount_ = 0;
			rushFlag_ = false;
		}
	}	
}
