#include "BossEnemy.h"
#include "TextureManager.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include "BossBullet.h"
#include "WinApp.h"
#include "Animation.h"
#include "Scroll.h"
#include "GlobalVariables.h"
#include "Audio.h"

#include <numbers>
#include <cmath>

BossEnemy::BossEnemy() {
	// キャラ
	charaTex_ = TextureManager::Load("BossEnemy.png");
	bossFunnelTex_ = TextureManager::Load("parpleBoss.png");
	bossRushTex_ = TextureManager::Load("blueBoss.png");
	bossNoneActionTex_ = TextureManager::Load("gleyBoss.png");
	parasiteTex_ = TextureManager::Load("bossbreak.png");
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
	// 方向
	directArrowTex_ = TextureManager::Load("BossMarker.png");
#pragma region サウンドテクスチャ
	// ファンネル
	funnelSEHandle_ = Audio::GetInstance()->LoadWave("music/FunnelSE.mp3");
	// 突進
	rushSEHandle_ = Audio::GetInstance()->LoadWave("music/RushSound.mp3");
	rushAlertSEHandle_ = Audio::GetInstance()->LoadWave("music/RushAlert.mp3");
	rushSpawnSEHandle_ = Audio::GetInstance()->LoadWave("music/rushSpawnSE.mp3");
	// 射撃
	bulletSEHandle_ = Audio::GetInstance()->LoadWave("music/bossBulletSE.mp3");
#pragma endregion
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
	prevAction_ = 20;
	behaviorRand_ = 20;
}

void BossEnemy::BossDirection() 
{
	Vector2 ankerPoint = nowPlayerPos_;
	Vector2 distance = {1280.0f, 720.0f};

	Vector2 leftTop = {ankerPoint.x - distance.x, ankerPoint.y - distance.y};

	Vector2 rightBottom = {ankerPoint.x + distance.x, ankerPoint.y + distance.y};

	Vector2 player2Boss = {pos_ - nowPlayerPos_};

	float dist = 200.0f;

	if (pos_.x < leftTop.x || pos_.x > rightBottom.x ||
		pos_.y < leftTop.y || pos_.y > rightBottom.y) 
	{	
		isScreenOut_ = true;
	} 
	else 
	{
		isScreenOut_ = false;
	}

	directArrowPos_ = MyMath::Normalize(player2Boss);
	directArrowPos_ = {directArrowPos_.x * dist, directArrowPos_.y * dist};
	//directSprite_->SetPosition(directArrowPos_ + nowPlayerPos_);

	float rotation = std::atan2(player2Boss.y, player2Boss.x);
	directSprite_->SetRotation(rotation);
}

void BossEnemy::GenerateBullet(Vector2& velocity, float speedValue) 
{
	// 生成・初期化
	BossBullet* newBullet = new BossBullet();
	newBullet->Initialize(bulletTex_, GetPosition(), velocity);
	newBullet->SetBulletSpeed(speedValue);
	// リストに追加
	bullets_.push_back(newBullet);
	Audio::GetInstance()->PlayWave(bulletSEHandle_, false, 0.1f);
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
	newFunnel->SetSE(funnelSEHandle_);
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
	// 行動系
	actionTimer_ = 0;
	kActionCoolTime_ = 60 * 5;
	// アクション中か
	isActionNow_ = false;
	// テーブルのラスト
	isLastAction_ = false;
	// 上下クロスのフラグ
	isCrossForm_ = false;

	sprite_.reset(
	    Sprite::Create(charaTex_, {pos_.x, pos_.y}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	rushSprite_.reset(
	    Sprite::Create(rushPointTex_, {0, 0}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	hpSprite_.reset(Sprite::Create(
	    hpTex_, {float(WinApp::kWindowWidth / 2), 60.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	hpShadowSprite_.reset(Sprite::Create(
	    hpShadowTex_, {float(WinApp::kWindowWidth / 2), 60.0f}, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));

	directSprite_.reset(Sprite::Create(
	    directArrowTex_, {directArrowPos_.x, directArrowPos_.y}, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));

	directSprite_->SetSize(Vector2(directSprite_->GetSize().x / 2, directSprite_->GetSize().x / 2));

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
	ImGui::Text(" %f : %f", directArrowPos_.x, directArrowPos_.y);
	ImGui::Text(" %d ", isLastAction_);
	ImGui::Text("isScreenOut : %d", isScreenOut_);
	ImGui::End();
#endif // _DEBUG

	/// 死ぬ処理
	if (isAlive_ && hp_ <= 0) {
		isParasite_ = true;
		isAlive_ = false;
	}

	ScreenPosInitialize();

	BossDirection();

	if (!isParasite_) {
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
		Animation::Anime(animationTimer, animationNumber, animationScene, oneTime);

	}
	if (isParasite_) {
		sprite_->SetTextureHandle(parasiteTex_);
		Animation::Anime(collapseAniTimer, animationNumber, collapseAniScene, collapseAnioneTime);
		isDamageShake = true;
		amplitNum = 18;
		MyMath::ShakeUpdate(shakeVelo_, isDamageShake, amplitNum);

		if (collapseAniTimer >= collapseAniScene * collapseAnioneTime) {

			isDead_ = true;
		}
	}
	
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

	ScDirect_ = directArrowPos_ - scroll->GetAddScroll() + sceneVelo;
	//directSprite_->SetPosition(ScDirect_);
	directSprite_->SetPosition(ScDirect_ + nowPlayerPos_);
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

	if (isScreenOut_ && isAlive_) {
		directSprite_->Draw();
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
	if (hp_ <= this->SetMaxHp / 2) {
		int coolTimeSecond = 4;
		kActionCoolTime_ = 60 * coolTimeSecond;
	} else {
		int coolTimeSecond = 6;
		kActionCoolTime_ = 60 * coolTimeSecond;
	}

	if (behavior_ == Behavior::kRoot && isAlive_) {
		actionTimer_++;
		if (actionTimer_ >= kActionCoolTime_) {
			ActionTable();
			actionTimer_ = 0;			
		}
	}
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_A)) {
		// actions_.push_back(Behavior::kRoot);
		actions_.push_back(Behavior::kFunnel);
		actions_.push_back(Behavior::kRushAlert);
	}
	if (input_->TriggerKey(DIK_S)) {
		// actions_.push_back(Behavior::kRoot);
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

#endif // DEBUG

	// リクエストの読み込み
	if (behavior_ != actions_.back()) {
		behaviorRequest_ = actions_.back();
	}
}

void BossEnemy::ActionTable() 
{
	prevAction_ = behaviorRand_;
	behaviorRand_ = rand() % 5;

	if (prevAction_ == behaviorRand_) {
		return;
	}

	/// 下から順番に呼び出す
	switch (behaviorRand_) {
	case 0:
		actions_.push_back(Behavior::kBarrage);
		actions_.push_back(Behavior::kGuided);
		actions_.push_back(Behavior::kFunnel);
		actions_.push_back(Behavior::kRushAlert);

		break;
	case 1:
		actions_.push_back(Behavior::kRushAlert);
		actions_.push_back(Behavior::kCross);
		actions_.push_back(Behavior::kFunnel);
		break;
	case 2:
		actions_.push_back(Behavior::kBarrage);
		actions_.push_back(Behavior::kRushAlert);
		break;
	case 3:
		actions_.push_back(Behavior::kRushAlert);
		actions_.push_back(Behavior::kFunnel);

		break;
	case 4:
		actions_.push_back(Behavior::kRushAlert);
		actions_.push_back(Behavior::kBarrage);
		actions_.push_back(Behavior::kCross);
		actions_.push_back(Behavior::kBarrage);

		break;
	}
}

void BossEnemy::RootUpdate() {
	ActionControl();

}

void BossEnemy::RootInitialize() {
	sprite_->SetTextureHandle(bossNoneActionTex_);
	actionTimer_ = 0;
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
