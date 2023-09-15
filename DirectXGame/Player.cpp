#include "Player.h"
#include "Animation.h"
#include "Audio.h"
#include "GlobalVariables.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "Scroll.h"
#include "TextureManager.h"
#include "WinApp.h"
#include <cassert>
#include <cmath>
#include <complex>

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() {}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player() {}

#pragma region 初期化系

/// <summary>
/// 初期化処理
/// </summary>
void Player::Initialize() {
	cursorTex_ = TextureManager::Load("Marker_ver2_0.png");
	cursor_.reset(
	    Sprite::Create(cursorTex_, {-20.0f, -20.0f}, {1.0f, 1.0f, 1.0f, 0.25f}, {0.5f, 0.5f}));
	cursor_->SetTextureRect({0.0f, 0.0f}, {128.0f, 128.0f});
	cursor_->SetSize({64.0f, 64.0f});
	// 入力系
	input_ = Input::GetInstance();
	if (isGameStart) {
		// 初期位置(仮)
		pos_ = {-200.0f, WinApp::kWindowHeight / 2};
	}
	//
	else if (!isGameStart) {
		// 初期位置(仮)
		pos_ = {(WinApp::kWindowWidth / 2) - 500.0, (WinApp::kWindowHeight / 2) + 100.0f};
	}
	clickPlayerPos_ = pos_;
	markerPos_ = pos_;
	preMarkerPos_ = markerPos_;

	// 半径の設定
	radius_ = 64.0f;

	// 無敵関係の初期化
	invisibleTimeCount_ = 0;
	isInvisible_ = false;

	BulletSEHandle_ = Audio::GetInstance()->LoadWave("music/p_bullet02.wav");
	DamageSEHandle_ = Audio::GetInstance()->LoadWave("music/p_Damage.wav");
	collapseHandle_ = Audio::GetInstance()->LoadWave("music/Collapse.wav");
	markerSetSEHandle_ = Audio::GetInstance()->LoadWave("music/markerSet.wav");

	// アニメーションで使う変数の初期化
	AnimationValueInitialize();

	sprite_.release();
	// プレイヤーののテクスチャ読み込み
	charaTex_ = TextureManager::Load("Player_ver2.png");
	//
	playerBreakTex_ = TextureManager::Load("playerBreak.png");

	// プレイヤーのスプライトの生成
	sprite_.reset(Sprite::Create(charaTex_, pos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// プレイヤーのサイズ設定
	sprite_->SetSize({radius_ * 2, radius_ * 2});

	// 弾系の初期化
	BulletInitialize();

	// 尻尾系の初期化
	TailInitialize();

	// UI系の初期化
	UIInitialize();

	// マーカーの初期化
	MarkerInitialize();
#pragma region 予測線
	// プレイヤーののテクスチャ読み込み
	predictionLineTex_ = TextureManager::Load("white1x1.png");
	// プレイヤーのスプライトの生成
	predictionLine_[0].reset(Sprite::Create(
	    predictionLineTex_, predictionLinePos_[0], {0.5f, 1.0f, 0.5f, 0.25f}, {0.0f, 0.5f}));
	predictionLine_[1].reset(Sprite::Create(
	    predictionLineTex_, predictionLinePos_[0], {0.5f, 1.0f, 0.5f, 0.25f}, {0.0f, 0.5f}));
	predictionLine_[0]->SetSize({1920.0f, 5.0f});
	predictionLine_[1]->SetSize({1920.0f, 5.0f});

#pragma endregion

	// 3本の尻尾の追加
	for (int i = 0; i < 3; i++) {
		AddTails();
	}
	//
#ifdef _DEBUG
	m1.reset(Sprite::Create(markerTex_, markerPos_, {1.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}));
	m2.reset(Sprite::Create(markerTex_, markerPos_, {0.0f, 1.0f, 0.0f, 1.0f}, {0.5f, 0.5f}));
	p1.reset(Sprite::Create(markerTex_, markerPos_, {0.0f, 0.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	p2.reset(Sprite::Create(markerTex_, markerPos_, {0.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}));
	origin_.reset(Sprite::Create(markerTex_, markerPos_, {0.5f, 0.5f, 0.5f, 1.0f}, {0.5f, 0.5f}));

	m1->SetTextureRect({0.0f, 0.0f}, {128.0f, 128.0f});
	m1->SetSize({64.0f, 64.0f});

	m2->SetTextureRect({0.0f, 0.0f}, {128.0f, 128.0f});
	m2->SetSize({64.0f, 64.0f});

	p1->SetTextureRect({0.0f, 0.0f}, {128.0f, 128.0f});
	p1->SetSize({64.0f, 64.0f});

	p2->SetTextureRect({0.0f, 0.0f}, {128.0f, 128.0f});
	p2->SetSize({64.0f, 64.0f});

	origin_->SetTextureRect({0.0f, 0.0f}, {128.0f, 128.0f});
	origin_->SetSize({64.0f, 64.0f});

#endif // _DEBUG

	// グローバル変数の初期化処理
	InitializeGrobalVariables();
	if (isGameStart) {
		clickPos_ = {700.0f, WinApp::kWindowHeight / 2};
		yoClickPos_ = clickPos_;
		// カーソルの位置取得
		// GetCursor();
		CursourCalicurate();

		MarkerControl();

		// クリックしたときの位置を取得
		clickPlayerPos_ = pos_;
		M2AddRadian[0] = yoM2AddRadian[0];
		M2AddRadian[1] = yoM2AddRadian[1];
		M2AddRadian[2] = yoM2AddRadian[2];
		W2AddRadian[0] = yoW2AddRadian[0];
		W2AddRadian[1] = yoW2AddRadian[1];
		W2AddRadian[2] = yoW2AddRadian[2];
		originPos_ = yoOriginPos_;

		//
		ToMarkerMoveInitialize();

		//
		RootRotateMoveInitialize();

		// RootRotateMove2();

		ismarkerMove_ = true;
		predictionLineSize[0] = {0.0f, 3.0f};
		predictionLineSize[1] = {0.0f, 3.0f};

		// マーカーの動き処理
		MarkerMovement();

	}

	else if (!isGameStart) {
		clickPos_ = {(WinApp::kWindowWidth / 2) + 100.0f, (WinApp::kWindowHeight / 2) + 100.0f};
		yoClickPos_ = clickPos_;
		// カーソルの位置取得
		// GetCursor();
		CursourCalicurate();

		MarkerControl();

		// クリックしたときの位置を取得
		clickPlayerPos_ = pos_;
		M2AddRadian[0] = yoM2AddRadian[0];
		M2AddRadian[1] = yoM2AddRadian[1];
		M2AddRadian[2] = yoM2AddRadian[2];
		W2AddRadian[0] = yoW2AddRadian[0];
		W2AddRadian[1] = yoW2AddRadian[1];
		W2AddRadian[2] = yoW2AddRadian[2];
		originPos_ = yoOriginPos_;

		//
		ToMarkerMoveInitialize();

		//
		RootRotateMoveInitialize();

		// RootRotateMove2();

		ismarkerMove_ = true;
		predictionLineSize[0] = {0.0f, 3.0f};
		predictionLineSize[1] = {0.0f, 3.0f};

		// マーカーの動き処理
		MarkerMovement();
	}
}

/// <summary>
/// 尻尾に関する初期化処理
/// </summary>
void Player::TailInitialize() {
	tails_.resize(0);
	// ミドリ尻尾のテクスチャ読み込み
	tailTexture_[0] = TextureManager::Load("Cannon_ver2.png");
	// 黄色尻尾のテクスチャ読み込み
	tailTexture_[1] = TextureManager::Load("Cannon_ver2_Safe.png");
	// 赤尻尾のテクスチャ読み込み
	tailTexture_[2] = TextureManager::Load("Cannon_ver2_Worning.png");
	// 崩れ尻尾のテクスチャ読み込み
	tailTexture_[3] = TextureManager::Load("CollapseCannon.png");
}

/// <summary>
/// 弾に関する初期化処理
/// </summary>
void Player::BulletInitialize() {
	bullets_.resize(0);
	// 弾のテクスチャ読み込み
	bulletTexture_ = TextureManager::Load("Bullet.png");
	// 弾パーティクルのテクスチャ読み込み
	bulletParticle_ = TextureManager::Load("bulletParticle.png");
}

/// <summary>
///  アニメーションに使う変数の初期化処理
/// </summary>
void Player::AnimationValueInitialize() {
	animationTimer = 0;
	animationNumber = 0;
	animationScene = 4;
	oneTime = 20;
	isAnimation = true;
	tailAniTimer = 0;
	tailAniNumber = 0;
	tailAniScene = 3;
	tailAnioneTime = 6;

	tailCollapseAniTimer = 0;
	tailCollapseAniNumber = 0;
	tailCollapseAniScene = 7;
	tailCollapseAnioneTime = 3;

	playerBreakAniTimer = 0;
	playerBreakAniNumber = 0;
	playerBreakAniScene = 8;
	playerBreakAnioneTime = 6;
}

/// <summary>
/// UIに関する初期化処理
/// </summary>
void Player::UIInitialize() {
	UITailPos_[0] = {UIPlayerPos_.x - (80.0f * 3.0f)};
	for (int i = 1; i < 6; i++) {
		UITailPos_[i] = {UITailPos_[0].x + (i * 80.0f), UITailPos_[0].y};
	}
	// (仮) プレイヤーのuiスプライト生成
	playerUI_.reset(
	    Sprite::Create(charaTex_, UIPlayerPos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	for (int i = 0; i < 6; i++) {
		tailUI_[i].reset(
		    Sprite::Create(tailTexture_[0], UITailPos_[i], {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
		tailUI_[i]->SetSize({(radius_ * 2) * 1.25f, (radius_ * 2) * 1.25f});
	}

	// uiプレイヤーのサイズ設定
	playerUI_->SetSize({radius_ * 2, radius_ * 2});
	playerUI_->SetRotation(1.0f * 3.14f);
}

/// <summary>
/// マーカーに関する初期化処理
/// </summary>
void Player::MarkerInitialize() {
	// マーカーのテクスチャ読み込み
	markerTex_ = TextureManager::Load("Marker_ver2_0.png");
	ismarkerAnimation = true;
	// マーカーのスプライトの生成
	markerSprite_.reset(
	    Sprite::Create(markerTex_, markerPos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	markerRadius_ = 32.0f;
	markerSprite_->SetSize({markerRadius_ * 2, markerRadius_ * 2});
}

/// <summary>
/// グローバル変数の初期化処理
/// </summary>
void Player::RootRotateMoveInitialize() {
	isMtM1 = false;
	isM1tM2 = false;
	isM2tP = false;
	isPtP1 = false;
	isP1tP2 = false;
	isP2tM = false;

	root_t_ = 0.0f;
	isRootMove_ = false;
}

/// <summary>
/// 通常状態の回転の初期化処理
/// </summary>
void Player::ToMarkerMoveInitialize() {
	move_t_ = 0.0f;

	// 移動フラグの初期化
	isMove_ = true;
}

/// <summary>
/// マーカーへ向かう動きの初期化処理
/// </summary>
void Player::InitializeGrobalVariables() {
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Player";

	// 作ったグループにそれぞれアイテムを追加
	gloVars->CreateGroup(groupName);
	gloVars->AddItem(groupName, "Move_t_offset", move_t_offset);
	gloVars->AddItem(groupName, "MarkerLimit_", markerLimit_);
	gloVars->AddItem(groupName, "RootRotate_t_offset", rootRotate_t_offset);
	gloVars->AddItem(groupName, "Bullet_shot_Radian", BulletRadian);
	gloVars->AddItem(groupName, "bulletSpeed_", bulletSpeed_);
	gloVars->AddItem(groupName, "oneTime", oneTime);
	gloVars->AddItem(groupName, "UIPlayerPos_", UIPlayerPos_);
}
#pragma endregion

#pragma region 更新系
/// <summary>
/// 更新処理
/// </summary>
void Player::Update() {

	// グローバル変数の値を取得
	ApplyGrobalVariables();
	ScreenPosInitialize();
	// ひとまずの尻尾追加
#ifdef _DEBUG
	////////////////////////////////////////////////////
	if (input_->TriggerKey(DIK_1)) {
		AddTails();
	}

	// ひとまずの尻尾削除
	if (input_->TriggerKey(DIK_2)) {
		DeleteTails();
	}
	// スクロールのインスタンスを取得
	Scroll* scroll = Scroll::GetInstance();

	////////////////////////////////////////////////////
	// 通常状態の回転用の道順
	p1->SetPosition(W2AddRadian[1] - scroll->GetAddScroll());
	p2->SetPosition(W2AddRadian[2] - scroll->GetAddScroll());
	m2->SetPosition(M2AddRadian[2] - scroll->GetAddScroll());
	m1->SetPosition(M2AddRadian[1] - scroll->GetAddScroll());
	origin_->SetPosition(originPos_ - scroll->GetAddScroll());

	// ImGui
	ImGui::Begin("debug");
	ImGui::Text("%f", root_t_);
	ImGui::DragFloat("radian", &BulletRadian, 0.1f, 2.0f);
	ImGui::Text("%d Timer : %d", isInvisible_, invisibleTimeCount_);
	ImGui::Text("score : %d", scoreCOunt);
	ImGui::End();

#endif
	// 前フレームの位置を取得
	prePos_ = pos_;

	// 自機の回転を反映させる
	sprite_->SetRotation(std::atan2(direction_.y, direction_.x));

	// カーソルの位置取得
	GetCursor();
	CursourCalicurate();

	if (!isPlayerBreak) {
		AliveUpdate();
	} else if (isPlayerBreak) {
		Vector4 color = sprite_->GetColor();
		color.w -= 0.015f;
		sprite_->SetColor(color);
		sprite_->SetTextureHandle(playerBreakTex_);
		Animation::Anime(
		    playerBreakAniTimer, animationNumber, playerBreakAniScene, playerBreakAnioneTime);
		if (playerBreakAniTimer >= playerBreakAniScene * playerBreakAnioneTime) {
			isDead_ = true;
		}
	}


}

/// <summary>
/// 衝突処理
/// </summary>
void Player::OnCollision() {
	if (!isInvisible_) {
		Audio::GetInstance()->PlayWave(DamageSEHandle_, false, volume);
		// もし当たったらシェイクフラグを有効に
		isDamageShake = true;
		damageCount--;
		isDamage = true;

		// 揺れ幅を設定
		amplitNum = 30;
		color_ = {1.0f, 0.1f, 0.1f, 1.0f};
	}
}
// void Player::OnCollision() {
//	// もし当たったらシェイクフラグを有効に
//	isDamageShake = true;
//	// 揺れ幅を設定
//	amplitNum = 30;
//	// 尻尾を減らす
//	DeleteTails();
// }

/// <summary>
/// グローバル変数の更新処理
/// </summary>
void Player::ApplyGrobalVariables() {
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Player";
	// 作ったグループにあるアイテムから値を取得
	move_t_offset = gloVars->GetFloatValue(groupName, "Move_t_offset");
	markerLimit_ = gloVars->GetFloatValue(groupName, "MarkerLimit_");
	rootRotate_t_offset = gloVars->GetFloatValue(groupName, "RootRotate_t_offset");
	BulletRadian = gloVars->GetFloatValue(groupName, "Bullet_shot_Radian");
	bulletSpeed_ = gloVars->GetFloatValue(groupName, "bulletSpeed_");
	oneTime = gloVars->GetIntValue(groupName, "oneTime");
	UIPlayerPos_ = gloVars->GetVector2Value(groupName, "UIPlayerPos_");
}

/// <summary>
/// キー入力での移動処理
/// </summary>
void Player::KeyMove() { // 移動距離
	Vector2 move{};
	// 移動速度定数
	const float kMoveSpeed = 5.0f;
	//

	// 各キーを押したら移動する
	if (input_->PushKey(DIK_W)) {
		move.y = -1.0f;
	}
	if (input_->PushKey(DIK_A)) {
		move.x = -1.0f;
	}
	if (input_->PushKey(DIK_S)) {
		move.y = 1.0f;
	}
	if (input_->PushKey(DIK_D)) {
		move.x = 1.0f;
	}

	move = MyMath::Normalize(move);
	// 移動速度をかけて加算する
	pos_.x += move.x * kMoveSpeed;
	pos_.y += move.y * kMoveSpeed;
}

/// <summary>
/// 左クリックの処理
/// </summary>
void Player::LeftClickUpdate() {
	Audio::GetInstance()->PlayWave(markerSetSEHandle_, false, 0.045f);

	// 前フレームのマーカー位置を取得
	preMarkerPos_ = markerPos_;

	// マーカーの位置を現在のマウス位置に設定
	clickPos_ = yoClickPos_;

	MarkerControl();

	// クリックしたときの位置を取得
	clickPlayerPos_ = pos_;

	//
	ToMarkerMoveInitialize();
	M2AddRadian[0] = yoM2AddRadian[0];
	M2AddRadian[1] = yoM2AddRadian[1];
	M2AddRadian[2] = yoM2AddRadian[2];
	W2AddRadian[0] = yoW2AddRadian[0];
	W2AddRadian[1] = yoW2AddRadian[1];
	W2AddRadian[2] = yoW2AddRadian[2];
	originPos_ = yoOriginPos_;
	//
	RootRotateMoveInitialize();

	ismarkerMove_ = true;
}

void Player::CursourCalicurate() {
	RootRotateMove2();

	Vector2 FireBulletDirection = yoM2AddRadian[0] - yoW2AddRadian[0];
	float markerROtate = std::atan2(FireBulletDirection.y, FireBulletDirection.x);

	cursor_->SetRotation(markerROtate + (0.5f * 3.14f));
	predictionLine_[0]->SetRotation(markerROtate - 0.35f);
	predictionLine_[1]->SetRotation(markerROtate + 0.7f);
}

void Player::AudioStop() {

	if (Audio::GetInstance()->IsPlaying(BulletSEHandle_)) {
		Audio::GetInstance()->StopWave(BulletSEHandle_);
	}
	if (Audio::GetInstance()->IsPlaying(DamageSEHandle_)) {
		Audio::GetInstance()->StopWave(DamageSEHandle_);
	}
	if (Audio::GetInstance()->IsPlaying(collapseHandle_)) {
		Audio::GetInstance()->StopWave(collapseHandle_);
	}
	if (Audio::GetInstance()->IsPlaying(markerSetSEHandle_)) {
		Audio::GetInstance()->StopWave(markerSetSEHandle_);
	}
}

/// <summary>
/// カーソルの更新処理
/// </summary>
void Player::GetCursor() {
	// カーソルの位置の取得
	GetCursorPos(&mousePos);
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	LogicalToPhysicalPointForPerMonitorDPI(hwnd, &mousePos);
	//auto dpi = WinApp::GetInstance()->GetDpi();
	//auto rate = dpi / 96.0f;

	//mousePos.x *= LONG(rate);
	//mousePos.y *= LONG(rate);

	//SetCursorPos(mousePos.x, mousePos.y);
	// クライアントエリア座標に変換する
	ScreenToClient(hwnd, &mousePos);
	
	Scroll* scroll = Scroll::GetInstance();

	

	cursor_->SetPosition({float(mousePos.x), float(mousePos.y)});
	


	mousePos.x = mousePos.x + LONG(scroll->GetAddScroll().x);
	mousePos.y = mousePos.y + LONG(scroll->GetAddScroll().y);
	yoClickPos_.x = float(mousePos.x);
	yoClickPos_.y = float(mousePos.y);
}

/// <summary>
/// カーソルの制限処理
/// </summary>
void Player::MarkerControl() {
	// スクロールのインスタンス取得
	Scroll* scroll = Scroll::GetInstance();

	if (clickPos_.x < 0 + markerLimit_) {
		clickPos_.x = markerLimit_ + scroll->GetAddScroll().x;
	}

	else if (
	    clickPos_.x > (WinApp::kWindowWidth * 1) + scroll->GetEdgePos().x +
	                        (scroll->GetEdgePos().x - markerLimit_)) {
		clickPos_.x = (WinApp::kWindowWidth * 1) + scroll->GetEdgePos().x +
		                (scroll->GetEdgePos().x - markerLimit_);
	}

	if (clickPos_.y < 0 + markerLimit_) {
		clickPos_.y = markerLimit_ + scroll->GetAddScroll().y;
	}

	else if (
	    clickPos_.y > (WinApp::kWindowHeight * 1) + scroll->GetEdgePos().y +
	                        (scroll->GetEdgePos().y - markerLimit_)) {
		clickPos_.y = (WinApp::kWindowHeight * 1) + scroll->GetEdgePos().y +
		                (scroll->GetEdgePos().y - markerLimit_);
	}
}

/// <summary>
/// マーカーの動きの処理
/// </summary>
void Player::MarkerMovement() {
	if (!ismarkerMove_) {
		markerMove_t = 0.0f;

	}
	//
	else if (ismarkerMove_) {
		markerPos_ = MyMath::EaseInQuadF(markerMove_t, preMarkerPos_, clickPos_);

		MyMath::CountT(markerMove_t, 1.0f, ismarkerMove_, false, 0.1f);
	}

	Vector2 Mark2Pla = clickPlayerPos_ - clickPos_;
	Vector2 PMark2Pla = clickPlayerPos_ - preMarkerPos_;

	Vector2 FireBulletDirection = clickPos_ - originPos_;
	float markerROtate = std::atan2(FireBulletDirection.y, FireBulletDirection.x);

	markerSprite_->SetRotation(markerROtate + (0.5f * 3.14f));

	//}
}

/// <summary>
/// 弾の更新処理
/// </summary>
void Player::BulletUpdate() {
	// 弾の更新処理
	for (PlayerBullet* bullet : bullets_) {
		bullet->SetSceneVelo(sceneVelo);
		bullet->Update();

		/*ImGui::Begin("bullet");
		ImGui::Text("%d", bullet->GetIsDead());
		ImGui::End();*/

		if (pos_.x + kDeadOffset < bullet->GetPosition().x ||
		    pos_.x - kDeadOffset > bullet->GetPosition().x ||
		    pos_.y + kDeadOffset < bullet->GetPosition().y ||
		    pos_.y - kDeadOffset > bullet->GetPosition().y) {
			bullet->SetIsDead(true);
		}
	}
}

/// <summary>
/// 弾を削除する処理
/// </summary>
void Player::DeleteBulletUpdate() {
	// 弾の消去
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
}

/// <summary>
/// 尻尾の更新処理
/// </summary>
void Player::TailUpdate() {
	// 尻尾の更新処理
	for (Tail* tail : tails_) {
		tail->SetSceneVelo(sceneVelo);
		tail->SetBulletRad(BulletRadian);
		tail->Update();
	}
}

/// <summary>
/// 通常状態の回転処理
/// </summary>
void Player::RootRotateMoveUpdate() {
	if (isMtM1) {

		pos_ = MyMath::CatmullRom(
		    W2AddRadian[2], M2AddRadian[0], M2AddRadian[1], M2AddRadian[2], root_t_);
		direction_ = pos_ - prePos_;

		if (root_t_ >= 1.0f) {
			direction_ = M2AddRadian[1] - M2AddRadian[0];

			isMtM1 = false;
		}
		MyMath::CountT(root_t_, 0.0f, isM1tM2, true, rootRotate_t_offset);
	}
	//
	if (isM1tM2) {
		pos_ = MyMath::CatmullRom(
		    M2AddRadian[0], M2AddRadian[1], M2AddRadian[2], W2AddRadian[0], root_t_);
		direction_ = pos_ - prePos_;
		if (root_t_ >= 1.0f) {
			direction_ = M2AddRadian[2] - M2AddRadian[1];
			isM1tM2 = false;
		}
		MyMath::CountT(root_t_, 0.0f, isM2tP, true, rootRotate_t_offset);
	}
	//
	if (isM2tP) {
		pos_ = MyMath::CatmullRom(
		    M2AddRadian[1], M2AddRadian[2], W2AddRadian[0], W2AddRadian[1], root_t_);
		direction_ = pos_ - prePos_;
		if (root_t_ >= 1.0f) {
			direction_ = W2AddRadian[0] - M2AddRadian[1];
			isM2tP = false;
		}
		MyMath::CountT(root_t_, 0.0f, isPtP1, true, rootRotate_t_offset);
	}
	//
	if (isPtP1) {
		pos_ = MyMath::CatmullRom(
		    M2AddRadian[2], W2AddRadian[0], W2AddRadian[1], W2AddRadian[2], root_t_);
		direction_ = pos_ - prePos_;
		if (root_t_ >= 1.0f) {
			direction_ = W2AddRadian[1] - W2AddRadian[0];
			isPtP1 = false;
		}
		MyMath::CountT(root_t_, 0.0f, isP1tP2, true, rootRotate_t_offset);
	}

	if (isP1tP2) {
		pos_ = MyMath::CatmullRom(
		    W2AddRadian[0], W2AddRadian[1], W2AddRadian[2], M2AddRadian[0], root_t_);
		direction_ = pos_ - prePos_;
		if (root_t_ >= 1.0f) {
			direction_ = W2AddRadian[2] - W2AddRadian[1];
			isP1tP2 = false;
		}
		MyMath::CountT(root_t_, 0.0f, isP2tM, true, rootRotate_t_offset);
	}

	if (isP2tM) {
		pos_ = MyMath::CatmullRom(
		    W2AddRadian[1], W2AddRadian[2], M2AddRadian[0], M2AddRadian[1], root_t_);
		direction_ = pos_ - prePos_;
		if (root_t_ >= 1.0f) {
			// direction_ = M2AddRadian[0] - W2AddRadian[2];
			isP2tM = false;
		}
		MyMath::CountT(root_t_, 0.0f, isMtM1, true, rootRotate_t_offset);

		if (isMtM1) {

			pos_ = MyMath::CatmullRom(
			    W2AddRadian[2], M2AddRadian[0], M2AddRadian[1], M2AddRadian[2], root_t_);
			direction_ = pos_ - prePos_;
			MyMath::CountT(root_t_, 0.0f, isM1tM2, true, rootRotate_t_offset);
		}
	}

	for (Tail* tail : tails_) {
		tail->SetIsMove(true);
	}
}

/// <summary>
/// 通常時の回転パターン1
/// </summary>
void Player::RootRotateMove1() {
	// クリックしたときのプレイヤーの位置->前回押した位置のマーカー
	Vector2 preMark2Pos_distance;
	preMark2Pos_distance = preMarkerPos_ - clickPlayerPos_;

	Vector2 RootMoveRadius = clickPos_ - pos_;
	RootMoveRadius.x = RootMoveRadius.x / 2;
	RootMoveRadius.y = RootMoveRadius.y / 2;

	originPos_ = pos_ + RootMoveRadius;
	Vector2 O2PVector = pos_ - originPos_;

	Matrix3x3 moveMat = MyMath::MakeTranslateMatrix(O2PVector);

	float cross = MyMath::Cross(RootMoveRadius, preMark2Pos_distance);
	if (cross >= 0) {
		Matrix3x3 P1rotateMat = MyMath::MakeRotateMatrix(-radianOffset);
		P1rotateMat = MyMath::Multiply(moveMat, P1rotateMat);
		W2AddRadian[0] = clickPlayerPos_;

		W2AddRadian[1] = {originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};

		Matrix3x3 P2rotateMat = MyMath::MakeRotateMatrix(-radianOffset * 2);
		P1rotateMat = MyMath::Multiply(moveMat, P2rotateMat);
		W2AddRadian[2] = {originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};

		Matrix3x3 M2rotateMat = MyMath::MakeRotateMatrix(radianOffset);
		P1rotateMat = MyMath::Multiply(moveMat, M2rotateMat);
		M2AddRadian[2] = {originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};

		Matrix3x3 M1rotateMat = MyMath::MakeRotateMatrix(radianOffset * 2);
		P1rotateMat = MyMath::Multiply(moveMat, M1rotateMat);
		M2AddRadian[1] = {originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};
		M2AddRadian[0] = clickPos_;

	}
	//
	else if (cross < 0) {
		Matrix3x3 P1rotateMat = MyMath::MakeRotateMatrix(radianOffset);
		P1rotateMat = MyMath::Multiply(moveMat, P1rotateMat);
		W2AddRadian[0] = clickPlayerPos_;

		W2AddRadian[1] = {originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};

		Matrix3x3 P2rotateMat = MyMath::MakeRotateMatrix(radianOffset * 2);
		P1rotateMat = MyMath::Multiply(moveMat, P2rotateMat);
		W2AddRadian[2] = {originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};

		Matrix3x3 M2rotateMat = MyMath::MakeRotateMatrix(-radianOffset);
		P1rotateMat = MyMath::Multiply(moveMat, M2rotateMat);
		M2AddRadian[2] = {originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};

		Matrix3x3 M1rotateMat = MyMath::MakeRotateMatrix(-radianOffset * 2);
		P1rotateMat = MyMath::Multiply(moveMat, M1rotateMat);
		M2AddRadian[1] = {originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};
		M2AddRadian[0] = clickPos_;
	}
}

/// <summary>
/// 通常時の回転パターン2
/// </summary>
void Player::RootRotateMove2() {
	Vector2 Mark2Pla = pos_ - yoClickPos_;
	Vector2 PMark2Pla = pos_ - markerPos_;

	float cross = MyMath::Cross(Mark2Pla, PMark2Pla);
	Vector2 Ple2Mark = yoClickPos_ - pos_;
	Mark2Pla.x = Mark2Pla.x / 2;
	Mark2Pla.y = Mark2Pla.y / 2;

	if (cross > 0) {

		Matrix3x3 Mark2PlaRotateMat = MyMath::MakeRotateMatrix(-(3.14f * 0.75f));
		Matrix3x3 Mark2PlaMat = MyMath::MakeTranslateMatrix(Mark2Pla);
		Mark2PlaMat = MyMath::Multiply(Mark2PlaMat, Mark2PlaRotateMat);
		Vector2 Mark2W = {Mark2PlaMat.m[2][0], Mark2PlaMat.m[2][1]};
		yoOriginPos_.x = yoClickPos_.x + Mark2W.x / 2;
		yoOriginPos_.y = yoClickPos_.y + Mark2W.y / 2;
		yoW2AddRadian[0].x = yoOriginPos_.x + Mark2W.x / 2;
		yoW2AddRadian[0].y = yoOriginPos_.y + Mark2W.y / 2;

		Vector2 Origin2W;
		Origin2W.x = Mark2W.x / 2;
		Origin2W.y = Mark2W.y / 2;

		Matrix3x3 Origin2WMat = MyMath::MakeTranslateMatrix(Origin2W);

		Matrix3x3 Origin2WRotateMat = MyMath::MakeRotateMatrix(radianOffset);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		yoW2AddRadian[1] = {
		    yoOriginPos_.x + Origin2WRotateMat.m[2][0], yoOriginPos_.y + Origin2WRotateMat.m[2][1]};

		Origin2WRotateMat = MyMath::MakeRotateMatrix(radianOffset * 2);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		yoW2AddRadian[2] = {
		    yoOriginPos_.x + Origin2WRotateMat.m[2][0], yoOriginPos_.y + Origin2WRotateMat.m[2][1]};

		yoM2AddRadian[0] = yoClickPos_;

		Origin2WRotateMat = MyMath::MakeRotateMatrix(-radianOffset * 2);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		yoM2AddRadian[1] = {
		    yoOriginPos_.x + Origin2WRotateMat.m[2][0], yoOriginPos_.y + Origin2WRotateMat.m[2][1]};

		Origin2WRotateMat = MyMath::MakeRotateMatrix(-radianOffset);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		yoM2AddRadian[2] = {
		    yoOriginPos_.x + Origin2WRotateMat.m[2][0], yoOriginPos_.y + Origin2WRotateMat.m[2][1]};
	}

	else if (cross <= 0) {

		Matrix3x3 Mark2PlaRotateMat = MyMath::MakeRotateMatrix(-(3.14f * 0.25f));
		Matrix3x3 Mark2PlaMat = MyMath::MakeTranslateMatrix(Mark2Pla);
		Mark2PlaMat = MyMath::Multiply(Mark2PlaMat, Mark2PlaRotateMat);
		Vector2 Mark2W = {Mark2PlaMat.m[2][0], Mark2PlaMat.m[2][1]};
		yoOriginPos_.x = yoClickPos_.x + Mark2W.x / 2;
		yoOriginPos_.y = yoClickPos_.y + Mark2W.y / 2;
		yoW2AddRadian[0].x = yoOriginPos_.x + Mark2W.x / 2;
		yoW2AddRadian[0].y = yoOriginPos_.y + Mark2W.y / 2;

		Vector2 Origin2W;
		Origin2W.x = Mark2W.x / 2;
		Origin2W.y = Mark2W.y / 2;

		Matrix3x3 Origin2WMat = MyMath::MakeTranslateMatrix(Origin2W);

		Matrix3x3 Origin2WRotateMat = MyMath::MakeRotateMatrix(radianOffset);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		yoW2AddRadian[1] = {
		    yoOriginPos_.x + Origin2WRotateMat.m[2][0], yoOriginPos_.y + Origin2WRotateMat.m[2][1]};

		Origin2WRotateMat = MyMath::MakeRotateMatrix(radianOffset * 2);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		yoW2AddRadian[2] = {
		    yoOriginPos_.x + Origin2WRotateMat.m[2][0], yoOriginPos_.y + Origin2WRotateMat.m[2][1]};

		yoM2AddRadian[0] = yoClickPos_;

		Origin2WRotateMat = MyMath::MakeRotateMatrix(-radianOffset * 2);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		yoM2AddRadian[1] = {
		    yoOriginPos_.x + Origin2WRotateMat.m[2][0], yoOriginPos_.y + Origin2WRotateMat.m[2][1]};

		Origin2WRotateMat = MyMath::MakeRotateMatrix(-radianOffset);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		yoM2AddRadian[2] = {
		    yoOriginPos_.x + Origin2WRotateMat.m[2][0], yoOriginPos_.y + Origin2WRotateMat.m[2][1]};
	}
}

/// <summary>
/// マーカーへ向かう動きの処理
/// </summary>
void Player::ToMarkerMoveUpdate() {
	RootRotateMoveInitialize();

	// ベジエ曲線のスタート位置計算
	bezierStartPos_ = MyMath::lerp(move_t_, clickPlayerPos_, preMarkerPos_);

	// ベジエ曲線の終わり位置計算
	bezierEndPos_ = MyMath::lerp(move_t_, preMarkerPos_, markerPos_);
	direction_ = bezierEndPos_ - bezierStartPos_;

	// 実際にプレイヤーの位置を計算
	pos_ = MyMath::lerp(move_t_, bezierStartPos_, bezierEndPos_);
	for (Tail* tail : tails_) {
		tail->SetIsMove(isMove_);
	}
	// 1.0になるまで加算
	MyMath::CountT(move_t_, 1.0f, isMove_, false, move_t_offset);

	if (!isMove_) {
		isMtM1 = true;
	}
	if (isMtM1) {
		pos_ = MyMath::CatmullRom(
		    W2AddRadian[2], M2AddRadian[0], M2AddRadian[1], M2AddRadian[2], root_t_);
		direction_ = pos_ - prePos_;

		MyMath::CountT(root_t_, 0.0f, isM1tM2, true, rootRotate_t_offset);
	}
}

void Player::AnimationUpdate() {
	// プレイヤーのアニメーション
	Animation::Anime(animationTimer, animationNumber, animationScene, oneTime);
	Animation::Anime(markerAniTimer, markerAniNumber, markerAniScene, markerAniOneTime);
	Animation::Anime(tailAniTimer, tailAniNumber, tailAniScene, tailAnioneTime);

	if (tails_.size() > 0) {
		if (tails_.back()->IsCollapse()) {
			if (tailCollapseAniTimer < tailCollapseAniScene * tailCollapseAnioneTime) {
				Animation::Anime(
				    tailCollapseAniTimer, tailCollapseAniNumber, tailCollapseAniScene,
				    tailCollapseAnioneTime);
			}
		}
	}
}
void Player::AliveUpdate() {
	// スクロールのインスタンスを取得
	Scroll* scroll = Scroll::GetInstance();

	// もし左クリックしたら
	if (input_->IsTriggerMouse(0) && !ismarkerMove_) {
		LeftClickUpdate();
		predictionLineSize[0] = {0.0f, 3.0f};
		predictionLineSize[1] = {0.0f, 3.0f};
	}

	// マーカーの動き処理
	MarkerMovement();

	// もしマーカーまで移動しきったら
	if (!isMove_) {
		// 通常状態の回転の処理
		RootRotateMoveUpdate();
	}
	// そうでないなら
	else if (isMove_) {
		// マーカーまでの移動処理
		ToMarkerMoveUpdate();
	}

	// 尻尾の更新
	TailUpdate();

	// 弾の削除処理
	DeleteBulletUpdate();

	// 弾の更新処理
	BulletUpdate();

	// マーカーの位置を反映させる
	markerSprite_->SetPosition(markerPos_ - scroll->GetAddScroll() + sceneVelo);
	// predictionLinePos_[0] = markerPos_;
	// predictionLinePos_[1] = markerPos_;
	predictionLinePos_[0].x = float(mousePos.x);
	predictionLinePos_[0].y = float(mousePos.y);
	predictionLinePos_[1].x = float(mousePos.x);
	predictionLinePos_[1].y = float(mousePos.y);

	predictionLine_[0]->SetPosition(predictionLinePos_[0] - scroll->GetAddScroll() + sceneVelo);
	predictionLine_[1]->SetPosition(predictionLinePos_[1] - scroll->GetAddScroll() + sceneVelo);

	AnimationUpdate();

	MyMath::ShakeUpdate(shakeVelo_, isDamageShake, amplitNum);

	sprite_->SetColor(color_);
	ScreenPos += shakeVelo_;
	// ベースの更新処理
	BaseCharacter::Update();
	if (isDamage) {
		color_ = {1.0f, 1.0f, 1.0f, 1.0f};
		isDamage = false;
	}
	if (isInvisible_) {
		invisibleTimeCount_ += 1;
		color_ = {1.0f, 0.7f, 0.7f, 0.8f};
	}

	if (invisibleTimeCount_ == kInvisibleTimer_) {
		invisibleTimeCount_ = 0;
		isInvisible_ = false;
		color_ = {1.0f, 1.0f, 1.0f, 1.0f};
	}

	if (tails_.size() > 0) {
		tails_.back()->SetHp(damageCount);
		if (damageCount <= 0) {
			damageCount = setDamageCount;
			isInvisible_ = true;
			tails_.back()->SetIsCollapse(true);
			Audio::GetInstance()->PlayWave(collapseHandle_, false, volume);
		}
		if (tails_.back()->IsCollapseAniEnd()) {
			DeleteTails();
		}
	}

	// 弾の消去
	tails_.remove_if([](Tail* tail) {
		if (tail->IsCollapseAniEnd()) {
			delete tail;
			return true;
		}
		return false;
	});

	playerUI_->SetPosition(UIPlayerPos_ + sceneVelo + shakeVelo_);

	if (tails_.size() > 0) {
		if (damageCount == 3) {
			tailUI_[int(tails_.size() - 1)]->SetTextureHandle(tailTexture_[0]);
		} else if (damageCount == 2) {
			tailUI_[int(tails_.size() - 1)]->SetTextureHandle(tailTexture_[1]);
		} else if (damageCount == 1) {
			tailUI_[int(tails_.size() - 1)]->SetTextureHandle(tailTexture_[2]);
		}
	}
	if (tails_.size() > 0) {
		for (int i = 0; i < tails_.size() - 1; i++) {
			tailUI_[i]->SetTextureHandle(tailTexture_[0]);
			tailUI_[i]->SetPosition(UITailPos_[i] + sceneVelo + shakeVelo_);
		}
	}

	if (tails_.size() > 1) {
		if (tails_.back()->IsCollapse()) {
			tailUI_[int(tails_.size() - 1)]->SetTextureHandle(tailTexture_[3]);
		}
		tailUI_[int(tails_.size() - 1)]->SetPosition(
		    UITailPos_[int(tails_.size() - 1)] + sceneVelo + shakeVelo_);
	}

	predictionLineSize[0].x = 1920;
	predictionLineSize[1].x = 1920;

	predictionLine_[0]->SetSize(predictionLineSize[0]);
	predictionLine_[1]->SetSize(predictionLineSize[1]);

	UITailPos_[0] = {UIPlayerPos_.x - (80.0f * 3.0f), UIPlayerPos_.y};
	for (int i = 1; i < 6; i++) {
		UITailPos_[i] = {UITailPos_[0].x + (i * 80.0f), UITailPos_[0].y};
	}
	if (isGameStart) {
		if (tails_.size() <= 0) {
			isPlayerBreak = true;
		}
	} 

}
#pragma endregion

#pragma region 描画系
/// <summary>
/// 描画処理
/// </summary>
void Player::Draw() {
	Scroll* scroll = Scroll::GetInstance();

	predictionLine_[0]->Draw();
	predictionLine_[1]->Draw();

	if (markerPos_.x < 0 + scroll->GetAddScroll().x ||
	    markerPos_.x > 1920 + scroll->GetAddScroll().x ||
	    markerPos_.y < 0 + scroll->GetAddScroll().y ||
	    markerPos_.y > 1080 + scroll->GetAddScroll().y) {
	} else {
		// クリックした位置マーカーの描画
		if (ismarkerAnimation) {
			Animation::DrawAnimation(markerSprite_.get(), markerPos_, markerAniNumber, markerTex_);
		}
		// もしアニメーションしないなら
		else if (!ismarkerAnimation) {
			markerSprite_->Draw();
		}
	}

	// 弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw();
	}

	// しっぽの描画
	tails_.reverse();
	for (Tail* tail : tails_) {
		tail->Draw();
	}
	tails_.reverse();

	// スプライトの描画
	BaseCharacter::Draw();
#ifdef _DEBUG
	/*m1->Draw();
	m2->Draw();
	p1->Draw();
	p2->Draw();
	origin_->Draw();*/
#endif // _DEBUG
}

/// <summary>
/// UI系の描画処理
/// </summary>
void Player::DrawUI() {
	if (tails_.size() > 0) {
		for (int i = 0; i < tails_.size() - 1; i++) {
			Animation::DrawAnimation(
			    tailUI_[i].get(), UITailPos_[i], tailAniNumber, tailTexture_[0]);
		}

		if (damageCount != 0) {
			Animation::DrawAnimation(
			    tailUI_[tails_.size() - 1].get(), UITailPos_[tails_.size() - 1], tailAniNumber,
			    tailTexture_[0]);
		}
		if (tails_.back()->IsCollapse()) {
			Animation::DrawAnimation(
			    tailUI_[tails_.size() - 1].get(), UITailPos_[tails_.size() - 1],
			    tailCollapseAniNumber, tailTexture_[0]);
		}
	}

	// UIプレイヤーの描画
	// Animation::DrawAnimation(playerUI_.get(), UIPlayerPos_, animationNumber, charaTex_);
}

void Player::DrawCursor() { cursor_->Draw(); }

#pragma endregion

#pragma region 追加や削除の関数
/// <summary>
/// 尻尾の追加
/// </summary>
void Player::AddTails() {
	if (tails_.size() < kMaxTail_) {
		// 新しい尻尾の生成
		Tail* newTail = new Tail();

		// そうでなければ
		if (tails_.size() != 0) {
			// 一個前の尻尾の位置を親として初期化
			if (!tails_.back()->IsCollapse()) {
				newTail->Initialize(
				    tailTexture_, &tails_.back()->GetPosition(), (tails_.back()->GetTailNo() + 1),
				    tails_.back()->IsFirePtr());
				tails_.back()->SetHp(setDamageCount);
				newTail->SetIsPlayersTail(true);
				// プレイヤーのポインタを設定
				newTail->SetPlayer(this);
				newTail->SetParticleTex(bulletParticle_);
				newTail->SetCollapseSEHandle(collapseHandle_);
				// リストに追加
				tails_.push_back(newTail);
			}
			//
		}
		// もし最初の尻尾なら
		else {
			// プレイヤーの位置を親として初期化
			newTail->Initialize(tailTexture_, &pos_, 0, &isMove_);
			newTail->SetIsPlayersTail(true);
			// プレイヤーのポインタを設定
			newTail->SetPlayer(this);
			newTail->SetParticleTex(bulletParticle_);
			newTail->SetCollapseSEHandle(collapseHandle_);

			// リストに追加
			tails_.push_back(newTail);
		}
	}
	//
	else if (tails_.size() == kMaxTail_) {
		damageCount = setDamageCount;
		scoreCOunt += 1;
	}
}

/// <summary>
/// 尻尾の削除
/// </summary>
void Player::DeleteTails() {
	if(isGameStart){
		if (tails_.size() > 0) {
			tails_.pop_back();
		}
	} else if (!isGameStart) {
		if (tails_.size() > 1) {
			tails_.pop_back();
		}

	}
}

/// <summary>
/// 弾の追加
/// </summary>
/// <param name="bullet">追加する弾</param>
void Player::AddBullets(PlayerBullet* bullet) {
	Audio::GetInstance()->PlayWave(BulletSEHandle_, false, volume);

	// 弾をリストに追加
	bullets_.push_back(bullet);
}
#pragma endregion
