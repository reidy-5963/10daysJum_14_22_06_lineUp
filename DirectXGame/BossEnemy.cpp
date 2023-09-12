#include "BossEnemy.h"
#include "TextureManager.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include "BossBullet.h"
#include "WinApp.h"
#include "Animation.h"
#include "Scroll.h"
#include "GlobalVariables.h"
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
	//pos_ = {float(WinApp::kWindowWidth), float(WinApp::kWindowHeight)};
	isAlive_ = true;
	isDead_ = false;
	float spawnY = 0;
	if (nowPlayerPos_.y >= 1080.0f) {
		spawnY = 1080.0f * 2.5f;
	} else {
		spawnY = 0 - (1080.0f / 2);
	}
	pos_ = {nowPlayerPos_.x, spawnY};
	prevPlayerPos_ = nowPlayerPos_;
	hp_ = SetMaxHp;
	animationTimer = 0;
	animationNumber = 0;
	animationScene = 4;
	oneTime = 5;
	isAnimation = true;
	actions_.push_back(Behavior::kRushAlert);
}

void BossEnemy::RandomActionManager() 
{ 
	//if (behavior_ == Behavior::kRoot && !isRush_ && !isFunnelAttackNow_) {
	//	actionTimer_++;
	//}
	//if (actionTimer_ == kActionCoolTime_) {
	//	if (isActionNow_ == kNowNone) {
	//		ActionTable();
	//		actionTimer_ = 0;
	//	}
	//}
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

	pos_ = {3000.0f, 3000.0f };

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

	hpSprite_->SetSize(Vector2(1200.0f, 60.0f));
	hpShadowSprite_->SetSize(Vector2(1200.0f, 60.0f));
	hpGaugeSize = hpSprite_->GetSize();
	MaxHpSize = hpSprite_->GetSize().x;
	// 当たり判定用の半径（サイズに合わせる）
	radius_ = 150.0f;
	// 適当にサイズ
	sprite_->SetSize(Vector2(radius_ * 2, radius_ * 2));
	particle_ = std::make_unique<ParticleManager>();
	particle_->Initialize(particleTex);

	actions_.push_back(Behavior::kRoot);
	InitializeGrobalVariables();
}

void BossEnemy::Update() 
{
	ApplyGrobalVariables();
	
#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::Text("%f : %f", nowPlayerPos_.x, nowPlayerPos_.y);
	ImGui::Text(" %d ", behavior_);
	ImGui::Text("actionTimer : %d\nCoolTime", actionTimer_, kActionCoolTime_);
	ImGui::Text(" %d ", isLastAction_);
	ImGui::End();
#endif // _DEBUG

	/// 死ぬ処理
	if (isAlive_ && hp_ <= 0) {
		isDead_ = true;
		isAlive_ = false;
	}

	ScreenPosInitialize();

	Animation::Anime(animationTimer, animationNumber, animationScene, oneTime);
	// リクエスト
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
		case BossEnemy::Behavior::kCross:
			CrossAttackInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}
	// リクエストごとの更新処理
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
	case BossEnemy::Behavior::kCross:
		CrossAttack();
		break;
	}
	// シェイク
	MyMath::ShakeUpdate(shakeVelo_, isDamageShake, amplitNum);
	// HPサイズ
	float hpSize = (float(hp_) / float(SetMaxHp)) * hpGaugeSize.x;
	// スプライトの設定
	hpSprite_->SetSize({hpSize, hpGaugeSize.y});
	hpSprite_->SetPosition({(1920 / 2) - (MaxHpSize - hpSize) / 2, 60.0f});

	ScreenPos += shakeVelo_;

	// スクロールのインスタンス取得
	Scroll* scroll = Scroll::GetInstance();
	ScPos = prevPlayerPos_ - scroll->GetAddScroll() + sceneVelo;
	
	rushSprite_->SetPosition(ScPos);
	//hp_ = SetMaxHp;
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
#ifdef _DEBUG
	ImGui::Begin("time");
	ImGui::Text("%d : %d", modeCount_, isFunnelAttackNow_);
	ImGui::Text("%d", actions_.back());
	ImGui::End();
#endif // _DEBUG

	for (BossBullet* bullet : bullets_) {
		bullet->SetSceneVelo(sceneVelo);
		bullet->Update();

		if (bullet->GetPosition().x < 0.0f || bullet->GetPosition().x > (1920.0f * 2) ||
		    bullet->GetPosition().y < 0.0f || bullet->GetPosition().y > (1080.0f * 2)) {
			bullet->SetIsDead(true);
		}
	}

	bullets_.remove_if([](BossBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

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
		if (funnel->GetPosition().x < 0.0f || funnel->GetPosition().x > (1920.0f * 2)||
		    funnel->GetPosition().y < 0.0f || funnel->GetPosition().y > (1080.0f * 2)) {
			funnel->SetIsDead(true);
		}
	}
}

void BossEnemy::ActionControl() 
{
	if (behavior_ == Behavior::kRoot && isAlive_) {
		actionTimer_++;
		if (actionTimer_ == kActionCoolTime_) {
			ActionTable();
			actionTimer_ = 0;			
		}
	}
	if (input_->TriggerKey(DIK_A)) {
		//actions_.push_back(Behavior::kRoot);
		actions_.push_back(Behavior::kFunnel);
		actions_.push_back(Behavior::kRushAlert);
	}
	if (input_->TriggerKey(DIK_S)) {
		//actions_.push_back(Behavior::kRoot);
		actions_.push_back(Behavior::kRushAlert);
		actions_.push_back(Behavior::kFunnel);
	}

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
	}
	/// 全方位
	if (input_->TriggerKey(DIK_G)) {
		actions_.push_back(Behavior::kFunnel);
		actions_.push_back(Behavior::kRushAlert);
		actions_.push_back(Behavior::kFunnel);
		actions_.push_back(Behavior::kRushAlert);
	}
	/// ファンネル
	if (input_->TriggerKey(DIK_H)) {
		actions_.push_back(Behavior::kRoot);
		actions_.push_back(Behavior::kCross);
		actions_.push_back(Behavior::kGuided);
		actions_.push_back(Behavior::kCross);
		actions_.push_back(Behavior::kFunnel);
	}
	/// 方向転換
	if (input_->TriggerKey(DIK_J)) {
		actions_.push_back(Behavior::kBarrage);
	}
	/// リスポーン
	if (input_->TriggerKey(DIK_K)) {
		RespownBoss();
	}

	// リクエストの読み込み
	if (behavior_ != actions_.back()) {
		behaviorRequest_ = actions_.back();
	}
}

void BossEnemy::RootInitialize() { actionTimer_ = 0; }

void BossEnemy::ActionTable() 
{
	int behaviorRand = rand() % 5;
	/// 下から順番に呼び出す
	switch (behaviorRand) {
	case 0:
		actions_.push_back(Behavior::kBarrage);
		actions_.push_back(Behavior::kGuided);
		actions_.push_back(Behavior::kFunnel);
		actions_.push_back(Behavior::kRushAlert);

		break;
	case 1:
		actions_.push_back(Behavior::kRushAlert);
		actions_.push_back(Behavior::kCross);
		actions_.push_back(Behavior::kBarrage);
		actions_.push_back(Behavior::kCross);
		actions_.push_back(Behavior::kFunnel);
		break;
	case 2:
		actions_.push_back(Behavior::kCross);
		actions_.push_back(Behavior::kRushAlert);
		actions_.push_back(Behavior::kBarrage);
		actions_.push_back(Behavior::kRushAlert);
		break;
	case 3:
		actions_.push_back(Behavior::kRushAlert);
		actions_.push_back(Behavior::kFunnel);
		break;
	case 4:

		break;
	}
}

void BossEnemy::RootUpdate() {
	ActionControl();

	if (isAlive_) {
		RandomActionManager();
	}
}

void BossEnemy::InitializeGrobalVariables() {
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Boss";

	// 作ったグループにそれぞれアイテムを追加
	gloVars->CreateGroup(groupName);
	gloVars->AddItem(groupName, "SetMaxHp", SetMaxHp);
}
void BossEnemy::ApplyGrobalVariables() {}
