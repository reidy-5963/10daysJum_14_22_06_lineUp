#include "Player.h"
#include "Animation.h"
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

/// <summary>
/// 初期化処理
/// </summary>
void Player::Initialize() {
	// 入力系
	input_ = Input::GetInstance();

	// 初期位置(仮)
	pos_ = {760, 320};
	clickPlayerPos_ = pos_;
	markerPos_ = pos_;
	preMarkerPos_ = markerPos_;
	animationTimer = 0;
	animationNumber = 0;
	animationScene = 4;
	oneTime = 20;
	isAnimation = true;
	// キャラのテクスチャ読み込み
	// charaTex_ = TextureManager::Load("Player.png");
	charaTex_ = TextureManager::Load("Player_ver2.png");

	// 無敵関係の初期化
	invisibleTimeCount_ = 0;
	isInvisible_ = false;

	// キャラのテクスチャ読み込み
	tailTexture_[0] = TextureManager::Load("Cannon_ver2.png");
	// キャラのテクスチャ読み込み
	tailTexture_[1] = TextureManager::Load("Cannon_ver2_Safe.png");
	// キャラのテクスチャ読み込み
	tailTexture_[2] = TextureManager::Load("Cannon_ver2_Worning.png");

	tailTexture_[3] = TextureManager::Load("CollapseCannon.png");

	// キャラのテクスチャ読み込み
	bulletTexture_ = TextureManager::Load("Bullet.png");
	bulletParticle_ = TextureManager::Load("bulletParticle.png");
	// スプライトの生成
	sprite_.reset(Sprite::Create(charaTex_, pos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	playerUI_.reset(
	    Sprite::Create(charaTex_, UIPlayerPos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	radius_ = 64.0f;
	sprite_->SetSize({radius_ * 2, radius_ * 2});
	playerUI_->SetSize({radius_ * 2, radius_ * 2});
	playerUI_->SetRotation(1.0f * 3.14f);
	// マーカーのテクスチャ読み込み
	markerTex_ = TextureManager::Load("Marker_ver2_0.png");
	ismarkerAnimation = true;
	// マーカーのスプライトの生成
	markerSprite_.reset(
	    Sprite::Create(markerTex_, markerPos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	float markerradius = 32.0f;
	markerSprite_->SetSize({markerradius * 2, markerradius * 2});
	// 1本目の追加
	AddTails();
	AddTails();
	AddTails();

	//
#ifdef _DEBUG
	m1.reset(Sprite::Create(markerTex_, markerPos_, {1.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}));
	m2.reset(Sprite::Create(markerTex_, markerPos_, {0.0f, 1.0f, 0.0f, 1.0f}, {0.5f, 0.5f}));
	p1.reset(Sprite::Create(markerTex_, markerPos_, {0.0f, 0.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	p2.reset(Sprite::Create(markerTex_, markerPos_, {0.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}));
	origin_.reset(Sprite::Create(markerTex_, markerPos_, {0.5f, 0.5f, 0.5f, 1.0f}, {0.5f, 0.5f}));

#endif // _DEBUG

	//
	InitializeGrobalVariables();
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update() {
	// グローバル変数の値を取得
	ApplyGrobalVariables();
	ScreenPosInitialize();
	// スクロールのインスタンスを取得
	Scroll* scroll = Scroll::GetInstance();
	// ひとまずの尻尾追加
	if (input_->TriggerKey(DIK_1)) {
		AddTails();
	}

	// ひとまずの尻尾削除
	if (input_->TriggerKey(DIK_2)) {
		DeleteTails();
	}

#ifdef _DEBUG
	////////////////////////////////////////////////////


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
	ImGui::End();

#endif
	// 前フレームの位置を取得
	prePos_ = pos_;

	// 自機の回転を反映させる
	sprite_->SetRotation(std::atan2(direction_.y, direction_.x));

	// カーソルの位置取得
	GetCursor();

	// もし左クリックしたら
	if (input_->IsTriggerMouse(0) && !ismarkerMove_) {
		LeftClickUpdate();
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
	DeleteBullet();

	// 弾の更新処理
	BulletUpdate();

	// マーカーの位置を反映させる
	markerSprite_->SetPosition(markerPos_ - scroll->GetAddScroll() + sceneVelo);

	// プレイヤーのアニメーション
	Animation::Anime(animationTimer, animationNumber, animationScene, oneTime);
	Animation::Anime(markerAniTimer, markerAniNumber, markerAniScene, markerAniOneTime);

	MyMath::ShakeUpdate(shakeVelo_, isDamageShake, amplitNum);

	sprite_->SetColor(color_);
	ScreenPos += shakeVelo_;
	// ベースの更新処理
	BaseCharacter::Update();

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
	}

	if (damageCount <= 0) {
		damageCount = setDamageCount;
		tails_.back()->SetIsCollapse(true);
	}

	if (tails_.back()->IsCollapseAniEnd()) {
		DeleteTails();
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
}

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
/// 描画処理
/// </summary>
void Player::Draw() {
	// クリックした位置マーカーの描画
	if (ismarkerAnimation) {
		Animation::DrawAnimation(markerSprite_.get(), markerPos_, markerAniNumber, markerTex_);
	} else if (!ismarkerAnimation) {
		markerSprite_->Draw();
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
	// m1->Draw();
	// m2->Draw();
	// p1->Draw();
	// p2->Draw();
	// origin_->Draw();
#endif // _DEBUG
}

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
			}
			//
			else if (tails_.back()->IsCollapse()) {
				newTail->Initialize(
				    tailTexture_, &(tails_.back() - 1)->GetPosition(),
				    ((tails_.back() - 1)->GetTailNo() + 1), (tails_.back() - 1)->IsFirePtr());
				(tails_.back() - 1)->SetHp(setDamageCount);

			}
		}
		// もし最初の尻尾なら
		else {
			// プレイヤーの位置を親として初期化
			newTail->Initialize(tailTexture_, &pos_, 0, &isMove_);
		}
		newTail->SetIsPlayersTail(true);
		// プレイヤーのポインタを設定
		newTail->SetPlayer(this);
		newTail->SetParticleTex(bulletParticle_);
		// リストに追加
		tails_.push_back(newTail);
	}
	if (tails_.size() == kMaxTail_) {
		damageCount = setDamageCount;
	}
	/*else if (tails_.size() == kMaxTail_) {
		tails_.back()->SetHp(setDamageCount);
	}*/
}

void Player::DeleteTails() {
	if (tails_.size() > 0) {
		tails_.pop_back();
	}
}

void Player::AddBullets(PlayerBullet* bullet) {
	// 弾をリストに追加
	bullets_.push_back(bullet);
}

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

void Player::ToMarkerMoveInitialize() {
	move_t_ = 0.0f;

	// 移動フラグの初期化
	isMove_ = true;
}

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

void Player::DrawUI() { 
	Animation::DrawAnimation(playerUI_.get(), UIPlayerPos_, animationNumber, charaTex_);
}

void Player::TailUpdate() {
	// 尻尾の更新処理
	for (Tail* tail : tails_) {
		tail->SetSceneVelo(sceneVelo);
		tail->SetBulletRad(BulletRadian);
		tail->Update();
	}
}

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

	//float cross = MyMath::Cross(Mark2Pla, PMark2Pla);
	////

	Vector2 FireBulletDirection = markerPos_ - originPos_;
	float markerROtate = std::atan2(FireBulletDirection.y, FireBulletDirection.x);
	//if (cross > 0) {
	//	markerSprite_->SetRotation(markerROtate - (0.5f * 3.14f));
	//} else if (cross <= 0) {
		markerSprite_->SetRotation(markerROtate + (0.5f * 3.14f));
	//}
}

void Player::OnCollision() {
	if (!isInvisible_) {
		// もし当たったらシェイクフラグを有効に
		isDamageShake = true;
		damageCount--;
		// 揺れ幅を設定
		amplitNum = 30;
		isInvisible_ = true;
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
}

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
}

void Player::GetCursor() {
	// カーソルの位置の取得
	GetCursorPos(&mousePos);
	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePos);
	Scroll* scroll = Scroll::GetInstance();
	mousePos.x = mousePos.x + LONG(scroll->GetAddScroll().x);
	mousePos.y = mousePos.y + LONG(scroll->GetAddScroll().y);
}

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

void Player::LeftClickUpdate() {
	// 前フレームのマーカー位置を取得
	preMarkerPos_ = markerPos_;

	// マーカーの位置を現在のマウス位置に設定
	clickPos_.x = float(mousePos.x);
	clickPos_.y = float(mousePos.y);

	MarkerControl();

	// クリックしたときの位置を取得
	clickPlayerPos_ = pos_;

	//
	ToMarkerMoveInitialize();

	//
	RootRotateMoveInitialize();

	RootRotateMove2();

	ismarkerMove_ = true;
}

void Player::DeleteBullet() {
	// 弾の消去
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
}

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

void Player::RootRotateMove2() {
	Vector2 Mark2Pla = clickPlayerPos_ - clickPos_;
	Vector2 PMark2Pla = clickPlayerPos_ - preMarkerPos_;

	float cross = MyMath::Cross(Mark2Pla, PMark2Pla);
	Vector2 Ple2Mark = clickPos_ - clickPlayerPos_;
	Mark2Pla.x = Mark2Pla.x / 2;
	Mark2Pla.y = Mark2Pla.y / 2;
	
		
	if (cross > 0) {

		Matrix3x3 Mark2PlaRotateMat = MyMath::MakeRotateMatrix(-(3.14f * 0.75f));
		Matrix3x3 Mark2PlaMat = MyMath::MakeTranslateMatrix(Mark2Pla);
		Mark2PlaMat = MyMath::Multiply(Mark2PlaMat, Mark2PlaRotateMat);
		Vector2 Mark2W = {Mark2PlaMat.m[2][0], Mark2PlaMat.m[2][1]};
		originPos_.x = clickPos_.x + Mark2W.x / 2;
		originPos_.y = clickPos_.y + Mark2W.y / 2;
		W2AddRadian[0].x = originPos_.x + Mark2W.x / 2;
		W2AddRadian[0].y = originPos_.y + Mark2W.y / 2;

		Vector2 Origin2W;
		Origin2W.x = Mark2W.x / 2;
		Origin2W.y = Mark2W.y / 2;

		Matrix3x3 Origin2WMat = MyMath::MakeTranslateMatrix(Origin2W);

		Matrix3x3 Origin2WRotateMat = MyMath::MakeRotateMatrix(radianOffset);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		W2AddRadian[1] = {
		    originPos_.x + Origin2WRotateMat.m[2][0], originPos_.y + Origin2WRotateMat.m[2][1]};

		Origin2WRotateMat = MyMath::MakeRotateMatrix(radianOffset * 2);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		W2AddRadian[2] = {
		    originPos_.x + Origin2WRotateMat.m[2][0], originPos_.y + Origin2WRotateMat.m[2][1]};

		M2AddRadian[0] = clickPos_;

		Origin2WRotateMat = MyMath::MakeRotateMatrix(-radianOffset * 2);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		M2AddRadian[1] = {
		    originPos_.x + Origin2WRotateMat.m[2][0], originPos_.y + Origin2WRotateMat.m[2][1]};

		Origin2WRotateMat = MyMath::MakeRotateMatrix(-radianOffset);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		M2AddRadian[2] = {
		    originPos_.x + Origin2WRotateMat.m[2][0], originPos_.y + Origin2WRotateMat.m[2][1]};
	}
	

	else if (cross <= 0) {

		Matrix3x3 Mark2PlaRotateMat = MyMath::MakeRotateMatrix(-(3.14f * 0.25f));
		Matrix3x3 Mark2PlaMat = MyMath::MakeTranslateMatrix(Mark2Pla);
		Mark2PlaMat = MyMath::Multiply(Mark2PlaMat, Mark2PlaRotateMat);
		Vector2 Mark2W = {Mark2PlaMat.m[2][0], Mark2PlaMat.m[2][1]};
		originPos_.x = clickPos_.x + Mark2W.x / 2;
		originPos_.y = clickPos_.y + Mark2W.y / 2;
		W2AddRadian[0].x = originPos_.x + Mark2W.x / 2;
		W2AddRadian[0].y = originPos_.y + Mark2W.y / 2;

		Vector2 Origin2W;
		Origin2W.x = Mark2W.x / 2;
		Origin2W.y = Mark2W.y / 2;

		Matrix3x3 Origin2WMat = MyMath::MakeTranslateMatrix(Origin2W);

		Matrix3x3 Origin2WRotateMat = MyMath::MakeRotateMatrix(radianOffset);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		W2AddRadian[1] = {
		    originPos_.x + Origin2WRotateMat.m[2][0], originPos_.y + Origin2WRotateMat.m[2][1]};

		Origin2WRotateMat = MyMath::MakeRotateMatrix(radianOffset * 2);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		W2AddRadian[2] = {
		    originPos_.x + Origin2WRotateMat.m[2][0], originPos_.y + Origin2WRotateMat.m[2][1]};

		M2AddRadian[0] = clickPos_;

		Origin2WRotateMat = MyMath::MakeRotateMatrix(-radianOffset * 2);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		M2AddRadian[1] = {
		    originPos_.x + Origin2WRotateMat.m[2][0], originPos_.y + Origin2WRotateMat.m[2][1]};

		Origin2WRotateMat = MyMath::MakeRotateMatrix(-radianOffset);
		Origin2WRotateMat = MyMath::Multiply(Origin2WMat, Origin2WRotateMat);
		M2AddRadian[2] = {
		    originPos_.x + Origin2WRotateMat.m[2][0], originPos_.y + Origin2WRotateMat.m[2][1]};
	}
}
