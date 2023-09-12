﻿#include "BossEnemy.h"
#include "TextureManager.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include "BossBullet.h"
#include "WinApp.h"
#include "Animation.h"
#include "Scroll.h"

#include <numbers>
#include <cmath>

BossEnemy::BossEnemy() {
	// キャラ
	charaTex_ = TextureManager::Load("BossEnemy.png");
	// 弾
	bulletTex_ = TextureManager::Load("BossBullet.png");
	// ファンネル
	funnelTex_ = TextureManager::Load("Fannel.png");
	particleTex = TextureManager::Load("FunnelParticle.png");
	// 突進先
	rushPointTex_ = TextureManager::Load("bossAttack.png");
	// HP
	hpTex_ = TextureManager::Load("HP_Out_Tex.png");
	hpShadowTex_ = TextureManager::Load("Hp_Gauge_Out_Tex.png");
}

void BossEnemy::RespownBoss() 
{ 
	pos_ = {float(WinApp::kWindowWidth), float(WinApp::kWindowHeight)};
	isAlive_ = true;
	isDead_ = false;
	prevBossPos_ = pos_;
	prevPlayerPos_ = nowPlayerPos_;
	hp_ = SetMaxHp;
	animationTimer = 0;
	animationNumber = 0;
	animationScene = 4;
	oneTime = 5;
	isAnimation = true;
}

void BossEnemy::RandomActionManager() 
{ 
	if (behavior_ == Behavior::kRoot && !isRush_ && !isFunnelAttackNow_) {
		actionTimer_++;
	}
	if (actionTimer_ == kActionCoolTime_) {
		if (isActionNow_ == kNowNone) {
			ActionTable();
			actionTimer_ = 0;
		}
	}
}

void BossEnemy::GenerateBullet(Vector2& velocity, float speedValue) 
{
	// 生成・初期化
	BossBullet* newBullet = new BossBullet();
	newBullet->Initialize(bulletTex_, GetPosition(), velocity);
	newBullet->SetBulletSpeed(speedValue);
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
		newFunnel->SetParticleTex(particleTex);
		newFunnel->Initialize(funnelTex_, type, prevBossPos_, endPoint);
		break;
	case BossFunnel::kVertical:
		endPoint = {pos_.x, nowPlayerPos_.y};
		newFunnel->SetParticleTex(particleTex);
		newFunnel->Initialize(funnelTex_, type, prevBossPos_, endPoint);
		break;
	}
	// リストに追加
	funnels_.push_back(newFunnel);
}

void BossEnemy::Initialize() 
{
	input_ = Input::GetInstance();

	pos_ = {-3000.0f, -3000.0f};

	//RespownBoss();
	animationTimer = 0;
	animationNumber = 0;
	animationScene = 4;
	oneTime = 5;
	isAnimation = true;

	sprite_.reset(
	    Sprite::Create(charaTex_, {pos_.x, pos_.y}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	rushSprite_.reset(
	    Sprite::Create(rushPointTex_, {0, 0}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	hpSprite_.reset(Sprite::Create(
	    hpTex_, {float(WinApp::kWindowWidth / 2), 60.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	hpShadowSprite_.reset(Sprite::Create(
	    hpShadowTex_, {float(WinApp::kWindowWidth / 2), 60.0f}, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));

	hpSprite_->SetSize(Vector2(1400.0f, 60.0f));
	hpShadowSprite_->SetSize(Vector2(1400.0f, 60.0f));
	hpGaugeSize = hpSprite_->GetSize();
	MaxHpSize = hpSprite_->GetSize().x;
	// 当たり判定用の半径（サイズに合わせる）
	radius_ = 150.0f;
	// 適当にサイズ
	sprite_->SetSize(Vector2(radius_ * 2, radius_ * 2));
	particle_ = std::make_unique<ParticleManager>();
	particle_->Initialize(particleTex);

	actions_.push_back(Behavior::kRoot);

}

void BossEnemy::Update() 
{
	ImGui::Begin("player");
	ImGui::Text("%f : %f", nowPlayerPos_.x, nowPlayerPos_.y);
	ImGui::End();

	if (isAlive_ && hp_ <= 0) {
		isDead_ = true;
		isAlive_ = false;
	}

	ScreenPosInitialize();

	Animation::Anime(animationTimer, animationNumber, animationScene, oneTime);

	if (behaviorRequest_) {
		// 行動変更
		behavior_ = behaviorRequest_.value();
		// 行動状態のカウントリセット
		modeCount_ = 0;
		// それぞれの初期化
		switch (behavior_) {
		case BossEnemy::Behavior::kRoot:
			RootInitialize();
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
		case BossEnemy::Behavior::kRushAlert:
			RushAlertInitialize();
			break;
		// ファンネル初期化
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
	case BossEnemy::Behavior::kRushAlert:
		RushAlert();
		break;
	/// ファンネル処理
	case BossEnemy::Behavior::kFunnel:
		FunnelAttack();
		break;
	}

	MyMath::ShakeUpdate(shakeVelo_, isDamageShake, amplitNum);

	float hpSize = (float(hp_) / float(SetMaxHp)) * hpGaugeSize.x;

	hpSprite_->SetSize({hpSize, hpGaugeSize.y});
	hpSprite_->SetPosition({(1920 / 2) - (MaxHpSize - hpSize) / 2, 60.0f});

	ScreenPos += shakeVelo_;

	// スクロールのインスタンス取得
	Scroll* scroll = Scroll::GetInstance();
	ScPos = prevPlayerPos_ - scroll->GetAddScroll() + sceneVelo;
	
	rushSprite_->SetPosition(ScPos);

	BulletUpdate();
	particle_->Update();

	// 座標設定
	BaseCharacter::Update();
}

void BossEnemy::Draw() 
{
	particle_->Draw();

	if (isRush_ || isRushNow_) {
		rushSprite_->Draw();
	}

	for (BossBullet* bullet : bullets_) {
		bullet->Draw();
	}
	for (BossFunnel* funnel : funnels_) {
		funnel->Draw();
	}
	// 描画
	BaseCharacter::Draw();

	if (isAlive_) {
		hpShadowSprite_->Draw();
		hpSprite_->Draw();
	}

}

void BossEnemy::OnCollision() { 	
	// もし当たったらシェイクフラグを有効に
	isDamageShake = true;
	// 揺れ幅を設定
	amplitNum = 12;
}

void BossEnemy::BulletUpdate() {

	ImGui::Begin("time");
	ImGui::Text("%d : %d", modeCount_,isFunnelAttackNow_);
	ImGui::Text("%d", actions_.back());
	ImGui::End();

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
		if (funnel->GetPosition().x < 0.0f || funnel->GetPosition().x > (1920.0f*2)||
		    funnel->GetPosition().y < 0.0f || funnel->GetPosition().y > (1080.0f * 2)) {
			funnel->SetIsDead(true);
		}
	}
}

void BossEnemy::ActionControl() 
{
	/// 突進起動キー処理
	if (input_->TriggerKey(DIK_D)) {
		actions_.push_back(Behavior::kBarrage);
		actions_.push_back(Behavior::kRushAlert);
		actions_.push_back(Behavior::kBarrage);
		actions_.push_back(Behavior::kRushAlert);
		actions_.push_back(Behavior::kBarrage);
		actions_.push_back(Behavior::kRushAlert);
		actions_.push_back(Behavior::kBarrage);
		actions_.push_back(Behavior::kRushAlert);
	}
	/// 誘導
	if (input_->TriggerKey(DIK_F)) {
		actions_.push_back(Behavior::kBarrage);
		actions_.push_back(Behavior::kGuided);
		actions_.push_back(Behavior::kFunnel);
		actions_.push_back(Behavior::kRushAlert);
			//behaviorRequest_ = actions_.back();
	}
	if (behavior_ != actions_.back()) {
		behaviorRequest_ = actions_.back();
		//isTest_ = true;
		//actions_.pop_front();
	}

	/// 全方位
	if (input_->TriggerKey(DIK_G)) {
		if (behavior_ != Behavior::kBarrage) {
			behaviorRequest_ = Behavior::kBarrage;
		}
	}
	/// ファンネル
	if (input_->TriggerKey(DIK_H)) {
		if (behavior_ != Behavior::kFunnel) {
			behaviorRequest_ = Behavior::kFunnel;
		}
	}
	/// 方向転換
	if (input_->TriggerKey(DIK_J)) {
		Vector2 directRotate = nowPlayerPos_ - pos_;
		// directRotate += pos_;
		sprite_->SetRotation(std::atan2(directRotate.y, directRotate.x));
	}
	/// リスポーン
	if (input_->TriggerKey(DIK_K)) {
		RespownBoss();
	}
}

void BossEnemy::RootInitialize() { actionTimer_ = 0; }

void BossEnemy::ActionTable() 
{
	int behaviorRand = rand() % 5;
	switch (behaviorRand) {
	case 0:
		break;
	case 1:
		RushAttackSetup();
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
	}
}

void BossEnemy::RootUpdate() 
{
	ActionControl();

	if (isAlive_) {
		RandomActionManager();
	}
}
