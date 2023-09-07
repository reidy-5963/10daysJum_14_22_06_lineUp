#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "TextureManager.h"
#include "WinApp.h"
#include <cassert>
#include <complex>
#include <cmath>
#include "Scroll.h"

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
	 
	// 当たり判定用に半径の設定
	radius_ = 32.0f;


	// キャラのテクスチャ読み込み
	charaTex_ = TextureManager::Load("Player.png");
	// キャラのテクスチャ読み込み
	tailTexture_ = TextureManager::Load("Cannon.png");
	// キャラのテクスチャ読み込み
	bulletTexture_ = TextureManager::Load("Bullet.png");

	// スプライトの生成
	sprite_.reset(Sprite::Create(charaTex_, pos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	// マーカーのテクスチャ読み込み
	markerTex_ = TextureManager::Load("Marker.png");

	// マーカーのスプライトの生成
	markerSprite_.reset(
	    Sprite::Create(markerTex_, markerPos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// 1本目の追加
	AddTails();

	//	
	m1.reset(Sprite::Create(markerTex_, markerPos_, {1.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}));
	m2.reset(Sprite::Create(markerTex_, markerPos_, {0.0f, 1.0f, 0.0f, 1.0f}, {0.5f, 0.5f}));
	p1.reset(Sprite::Create(markerTex_, markerPos_, {0.0f, 0.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	p2.reset(Sprite::Create(markerTex_, markerPos_, {0.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}));
	origin_.reset(Sprite::Create(markerTex_, markerPos_, {0.5f, 0.5f, 0.5f, 1.0f}, {0.5f, 0.5f}));
	//
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update() {
	Scroll* scroll = Scroll::GetInstance();

#ifdef _DEBUG 
//////////////////////////////////////////////////// 
	// ひとまずの尻尾追加
	if (input_->TriggerKey(DIK_1)) {
		AddTails();
	}

	// ひとまずの尻尾削除
	if (input_->TriggerKey(DIK_2)) {
		DeleteTails();
	}

////////////////////////////////////////////////////
	p1->SetPosition(PlayerAddRadian[0] - scroll->GetAddScroll());
	p2->SetPosition(PlayerAddRadian[1] - scroll->GetAddScroll());
	m2->SetPosition(markerAddRadian[1] - scroll->GetAddScroll());
	m1->SetPosition(markerAddRadian[0] - scroll->GetAddScroll());
	origin_->SetPosition(originPos_ - scroll->GetAddScroll());

	ImGui::Begin("debug");
	ImGui::Text("%f", root_t_);
	ImGui::DragFloat("radian", &BulletRadian, 0.1f, 2.0f);
	ImGui::End();

#endif

	// カーソルの位置取得
	CursorUpdate();

	// もし左クリックしたら
	if (input_->IsTriggerMouse(0)) {
		LeftClickUpdate();
	}
	prePos_ = pos_;

	// もしマーカーまで移動しきったら
	if (!isMove_) {

		if (isMtM1) {
			
			pos_ = MyMath::CatmullRom(
			    PlayerAddRadian[1], markerPos_, markerAddRadian[0], markerAddRadian[1], root_t_);
			direction_ = pos_ - prePos_;

			if (root_t_ >= 1.0f) {
				direction_ = markerAddRadian[0] - markerPos_;

				isMtM1 = false;
			}
			MyMath::CountT(root_t_, 0.0f, isM1tM2, true, rootTOffset);
		}
		//
		if (isM1tM2) {
			pos_ = MyMath::CatmullRom(
			    markerPos_, markerAddRadian[0], markerAddRadian[1], clickPlayerPos_, root_t_);
			direction_ = pos_ - prePos_;
			if (root_t_ >= 1.0f) {
				direction_ = markerAddRadian[1] - markerAddRadian[0];
				isM1tM2 = false;
			}
			MyMath::CountT(root_t_, 0.0f, isM2tP, true, rootTOffset);
		}
		//
		if (isM2tP) {
			pos_ = MyMath::CatmullRom(
			    markerAddRadian[0], markerAddRadian[1], clickPlayerPos_, PlayerAddRadian[0],
			    root_t_);
			direction_ = pos_ - prePos_;
			if (root_t_ >= 1.0f) {
				direction_ = clickPlayerPos_ - markerAddRadian[0];
				isM2tP = false;
			}
			MyMath::CountT(root_t_, 0.0f, isPtP1, true, rootTOffset);
		}
		//
		if (isPtP1) {
			pos_ = MyMath::CatmullRom(
			    markerAddRadian[1], clickPlayerPos_, PlayerAddRadian[0], PlayerAddRadian[1],
			    root_t_);
			direction_ = pos_ - prePos_;
			if (root_t_ >= 1.0f) {
				direction_ = PlayerAddRadian[0] - clickPlayerPos_;
				isPtP1 = false;
			}
			MyMath::CountT(root_t_, 0.0f, isP1tP2, true, rootTOffset);
		}	

		if (isP1tP2) {
			pos_ = MyMath::CatmullRom(
			    clickPlayerPos_, PlayerAddRadian[0], PlayerAddRadian[1], markerPos_, root_t_);
			direction_ = pos_ - prePos_;
			if (root_t_ >= 1.0f) {
				direction_ = PlayerAddRadian[1] - PlayerAddRadian[0];
				isP1tP2 = false;
			}
			MyMath::CountT(root_t_, 0.0f, isP2tM, true, rootTOffset);
		}

		if (isP2tM) {
			pos_ = MyMath::CatmullRom(
			    PlayerAddRadian[0], PlayerAddRadian[1], markerPos_, markerAddRadian[0], root_t_);
			direction_ = pos_ - prePos_;
			if (root_t_ >= 1.0f) {
				direction_ = markerPos_ - PlayerAddRadian[1];
				isP2tM = false;
			}
			MyMath::CountT(root_t_, 0.0f, isMtM1, true, rootTOffset);
		}
		
		////// ベジエ曲線のスタート位置計算
		//	// bezierStartPos_ = MyMath::lerp(root_t_, markerPos_, RotateRootPos_);
		//	//// ベジエ曲線の終わり位置計算
		//	// bezierEndPos_ = MyMath::lerp(root_t_, RotateRootPos_, clickPlayerPos_);
		//	// prePos_ = pos_;
		//	//// 実際にプレイヤーの位置を計算
		//	// pos_ = MyMath::lerp(root_t_, bezierStartPos_, bezierEndPos_);
		////}
		////
		////else if (isRootMove_) { 
		//	//// ベジエ曲線のスタート位置計算
		//	//bezierStartPos_ = MyMath::lerp(root_t_, clickPlayerPos_, preMarkerPos_);
		//	//// ベジエ曲線の終わり位置計算
		//	//bezierEndPos_ = MyMath::lerp(root_t_, preMarkerPos_, markerPos_);
		//	//
		//	//// 実際にプレイヤーの位置を計算
		//	//pos_ = MyMath::lerp(root_t_, bezierStartPos_, bezierEndPos_);
		////	pos_ = MyMath::CatmullRom(
		////	    markerPos_, RotateRootPos_, clickPlayerPos_, preMarkerPos_, root_t_);
		////	prePos_ = pos_;
		//	//CountT(root_t_, 0.0f, isRootMove_, false, 0.01f);
		////}
		for (Tail* tail : tails_) {
			tail->SetIsMove(true);
		}

	} 
	// そうでないなら
	else if (isMove_) {
		isMtM1 = false;
		isM1tM2 = false;
		isM2tP = false;
		isPtP1 = false;
		isP1tP2 = false;
		isP2tM = false;

		root_t_ = 0.0f;
		isRootMove_ = false;

		// 1.0になるまで加算
		MyMath::CountT(move_t_, 1.0f, isMove_, false, move_t_offset);
		if (!isMove_) {
			isMtM1 = true;
		}

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
	}

	// 尻尾の更新
	TailUpdate();

	// 弾の削除処理
	DeleteBullet();

	// 弾の更新処理
	BulletUpdate();

	// ベースの更新処理
	BaseCharacter::Update(); 
	
	// 自機の回転を反映させる
	sprite_->SetRotation(std::atan2(direction_.y, direction_.x));

	// マーカーの位置を反映させる
	markerSprite_->SetPosition(markerPos_ - scroll->GetAddScroll());

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
	// 弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw();
	}
	// しっぽの描画
	for (Tail* tail : tails_) {
		tail->Draw();
	}

	// クリックした位置マーカーの描画
	markerSprite_->Draw();

	// スプライトの描画
	BaseCharacter::Draw();
#ifdef _DEBUG
	//m1->Draw();
	//m2->Draw();
	//p1->Draw();
	//p2->Draw();
	//origin_->Draw();
#endif // _DEBUG
}

void Player::AddTails() {
	if (tails_.size() < kMaxTail_) {
		// 新しい尻尾の生成
		Tail* newTail = new Tail();

		// そうでなければ
		if (tails_.size() != 0) {
			// 一個前の尻尾の位置を親として初期化
			newTail->Initialize(
			    tailTexture_, &tails_.back()->GetPosition(), (tails_.back()->GetTailNo() + 1),
			    tails_.back()->IsFirePtr());
		}
		// もし最初の尻尾なら
		else {
			// プレイヤーの位置を親として初期化
			newTail->Initialize(tailTexture_, &pos_, 0, &isMove_);
		}

		// プレイヤーのポインタを設定
		newTail->SetPlayer(this);

		// リストに追加
		tails_.push_back(newTail);

	}
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

void Player::TailUpdate() {
	// 尻尾の更新処理
	for (Tail* tail : tails_) {
		tail->SetBulletRad(BulletRadian);
		tail->Update();
	}
}

void Player::OnCollision() { DeleteTails(); }

void Player::CursorUpdate() {
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
		bullet->Update();

		ImGui::Begin("bullet");
		ImGui::Text("%d", bullet->GetIsDead());
		ImGui::End();

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
	
	
	if (markerPos_.x < 0 + markerLimit_)
	{
		markerPos_.x = markerLimit_ + scroll->GetAddScroll().x;
	}

	else if (markerPos_.x > (WinApp::kWindowWidth * 2) + scroll->GetEdgePos().x + (scroll->GetEdgePos().x - markerLimit_)) 
	{
		markerPos_.x = (WinApp::kWindowWidth * 2) + scroll->GetEdgePos().x + (scroll->GetEdgePos().x - markerLimit_);
	}

	if (markerPos_.y < 0 + markerLimit_) 
	{
		markerPos_.y = markerLimit_ + scroll->GetAddScroll().y;
	} 

	else if (markerPos_.y > (WinApp::kWindowHeight * 2) + scroll->GetEdgePos().y + (scroll->GetEdgePos().y - markerLimit_))
	{
		markerPos_.y = (WinApp::kWindowHeight * 2) + scroll->GetEdgePos().y + (scroll->GetEdgePos().y - markerLimit_);
	}
}

void Player::LeftClickUpdate() {
	// 前フレームのマーカー位置を取得
	preMarkerPos_ = markerPos_;

	// マーカーの位置を現在のマウス位置に設定
	markerPos_.x = float(mousePos.x);
	markerPos_.y = float(mousePos.y);

	MarkerControl();

	// クリックしたときの位置を取得
	clickPlayerPos_ = pos_;
	// 線形補間の初期化
	move_t_ = 0.0f;

	// 移動フラグの初期化
	isMove_ = true;
	// 線形補間用tの初期化
	root_t_ = 0.0f;

	// クリックしたときのプレイヤーの位置->前回押した位置のマーカー
	Vector2 preMark2Pos_distance;
	preMark2Pos_distance = preMarkerPos_ - clickPlayerPos_;

	{ // （仮）
		Vector2 A2B = clickPlayerPos_ - markerPos_;
		A2B.x = A2B.x / 2;
		A2B.y = A2B.y / 2;

		Vector2 verticalA2B = {A2B.x, -A2B.y};
		if (A2B.x > 0) {
		}
		// 通常時の回転で使う四つ目の点を求める
		RotateRootPos_ = clickPlayerPos_ + A2B /*markerPos_ - preMark2Pos_distance*/;
		RotateRootPos_ += verticalA2B;
	}

	Vector2 RootMoveRadius = markerPos_ - pos_;
	RootMoveRadius.x = RootMoveRadius.x / 2;
	RootMoveRadius.y = RootMoveRadius.y / 2;

	originPos_ = pos_ + RootMoveRadius;
	Vector2 O2PVector = pos_ - originPos_;

	Matrix3x3 moveMat = MyMath::MakeTranslateMatrix(O2PVector);

	float cross = MyMath::Cross(RootMoveRadius, preMark2Pos_distance);
	if (cross >= 0) {
		Matrix3x3 P1rotateMat = MyMath::MakeRotateMatrix(-radianOffset);
		P1rotateMat = MyMath::Multiply(moveMat, P1rotateMat);

		PlayerAddRadian[0] = {
		    originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};

		Matrix3x3 P2rotateMat = MyMath::MakeRotateMatrix(-radianOffset * 2);
		P1rotateMat = MyMath::Multiply(moveMat, P2rotateMat);
		PlayerAddRadian[1] = {
		    originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};

		Matrix3x3 M2rotateMat = MyMath::MakeRotateMatrix(radianOffset);
		P1rotateMat = MyMath::Multiply(moveMat, M2rotateMat);
		markerAddRadian[1] = {
		    originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};

		Matrix3x3 M1rotateMat = MyMath::MakeRotateMatrix(radianOffset * 2);
		P1rotateMat = MyMath::Multiply(moveMat, M1rotateMat);
		markerAddRadian[0] = {
		    originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};


	}
	else if (cross < 0) {
		Matrix3x3 P1rotateMat = MyMath::MakeRotateMatrix(radianOffset);
		P1rotateMat = MyMath::Multiply(moveMat, P1rotateMat);

		PlayerAddRadian[0] = {
		    originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};

		Matrix3x3 P2rotateMat = MyMath::MakeRotateMatrix(radianOffset * 2);
		P1rotateMat = MyMath::Multiply(moveMat, P2rotateMat);
		PlayerAddRadian[1] = {
		    originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};

		Matrix3x3 M2rotateMat = MyMath::MakeRotateMatrix(-radianOffset);
		P1rotateMat = MyMath::Multiply(moveMat, M2rotateMat);
		markerAddRadian[1] = {
		    originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};

		Matrix3x3 M1rotateMat = MyMath::MakeRotateMatrix(-radianOffset * 2);
		P1rotateMat = MyMath::Multiply(moveMat, M1rotateMat);
		markerAddRadian[0] = {
		    originPos_.x + P1rotateMat.m[2][0], originPos_.y + P1rotateMat.m[2][1]};

	}


	// 四ツ目の点を求める
	//RotateRootPos_ = markerPos_ - preMark2Pos_distance;
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
