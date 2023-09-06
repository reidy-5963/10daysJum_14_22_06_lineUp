#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "TextureManager.h"
#include "WinApp.h"
#include <cassert>
#include <complex>
#include <cmath>

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

	// キャラのテクスチャ読み込み
	charaTex_ = TextureManager::Load("Player.png");
	// キャラのテクスチャ読み込み
	tailTexture_ = TextureManager::Load("Cannon.png");
	// キャラのテクスチャ読み込み
	bulletTexture_ = TextureManager::Load("Bullet.png");

	// スプライトの生成
	sprite_.reset(Sprite::Create(charaTex_, {720, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	// マーカーのテクスチャ読み込み
	markerTex_ = TextureManager::Load("Marker.png");

	// マーカーのスプライトの生成
	markerSprite_.reset(
	    Sprite::Create(markerTex_, {720, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	// 初期位置(仮)
	pos_ = {760, 320};
	clickPlayerPos_ = pos_;
	markerPos_ = pos_;
	preMarkerPos_ = markerPos_;

	radius_ = 32.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update() {
	// カーソルの位置の取得
	GetCursorPos(&mousePos);
	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePos);
	// もし左クリックしたら
	if (input_->IsTriggerMouse(0)) {
		// 前フレームのマーカー位置を取得
		preMarkerPos_ = markerPos_;

		// マーカーの位置を現在のマウス位置に設定
		markerPos_.x = float(mousePos.x);
		markerPos_.y = float(mousePos.y);
		// クリックしたときの位置を取得
		clickPlayerPos_ = pos_;
		// 線形補間の初期化
		move_t_ = 0.0f;

		//
		isMove_ = true;
		root_t_ = 0.0f;

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

		RotateRootPos_ = markerPos_ - preMark2Pos_distance;

	}
	
	// 
	if (!isMove_) {
		if (!isRootMove_) {

			// ベジエ曲線のスタート位置計算
			bezierStartPos_ = MyMath::lerp(root_t_, markerPos_, RotateRootPos_);

			// ベジエ曲線の終わり位置計算
			bezierEndPos_ = MyMath::lerp(root_t_, RotateRootPos_, clickPlayerPos_);

			prePos_ = pos_;

			// 実際にプレイヤーの位置を計算
			pos_ = MyMath::lerp(root_t_, bezierStartPos_, bezierEndPos_);
			
			if (root_t_ > 1.0f) {
				root_t_ = 0.0f;
				isRootMove_ = true;
			} else {
				root_t_ += 0.01f;
			}

		}
		//
		else if (isRootMove_) { 
			// ベジエ曲線のスタート位置計算
			bezierStartPos_ = MyMath::lerp(root_t_, clickPlayerPos_, preMarkerPos_);

			// ベジエ曲線の終わり位置計算
			bezierEndPos_ = MyMath::lerp(root_t_, preMarkerPos_, markerPos_);
			
			// 実際にプレイヤーの位置を計算
			pos_ = MyMath::lerp(root_t_, bezierStartPos_, bezierEndPos_);

			prePos_ = pos_;

			if (root_t_ > 1.0f) {
				root_t_ = 0.0f;
				isRootMove_ = false;

			} else {
				root_t_ += 0.01f;
			}
		}


		for (Tail* tail : tails_) {
			tail->SetIsMove(true);
		}

	} 
	//
	else if (isMove_) {
		root_t_ = 0.0f;
		isRootMove_ = false;

		// 1.0になるまで加算
		if ((move_t_ += 0.01f) >= 1.0f) {
			move_t_ = 1.0f;
			isMove_ = false;
		}

		// ベジエ曲線のスタート位置計算
		bezierStartPos_ = MyMath::lerp(move_t_, clickPlayerPos_, preMarkerPos_);

		// ベジエ曲線の終わり位置計算
		bezierEndPos_ = MyMath::lerp(move_t_, preMarkerPos_, markerPos_);

		prePos_ = pos_;

		// 実際にプレイヤーの位置を計算
		pos_ = MyMath::lerp(move_t_, bezierStartPos_, bezierEndPos_);
		for (Tail* tail : tails_) {
			tail->SetIsMove(isMove_);
		}
	}

	// スプライトに位置を反映させる
	sprite_->SetPosition(pos_);

	// 向きの計算
	Vector2 move = bezierEndPos_ - bezierStartPos_;
	sprite_->SetRotation(std::atan2(move.y, move.x));

	if (input_->TriggerKey(DIK_1) && tails_.size() < 7) {
		AddTails();
	}

	if (input_->TriggerKey(DIK_2) &&tails_.size() > 0) {
		DeleteTails();
	}


	for (Tail* tail : tails_) {
		tail->Update();
	}

	// 弾の消去
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	ImGui::Begin("debug");
	ImGui::Text("%f", root_t_);

	ImGui::End();

	markerSprite_->SetPosition(markerPos_);
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
}

void Player::AddTails() {
	// 新しい尻尾の生成
	Tail* newTail = new Tail();
	
	// そうでなければ
	if (tails_.size() != 0) {
		// 一個前の尻尾の位置を親として初期化
		newTail->Initialize(
		    tailTexture_, tails_.back()->GetPosition(), (tails_.back()->GetTailNo() + 1));
	} 	
	// もし最初の尻尾なら
	else {
		// プレイヤーの位置を親として初期化
		newTail->Initialize(tailTexture_, &pos_, 0);
	}
	
	// プレイヤーのポインタを設定
	newTail->SetPlayer(this);
	
	// リストに追加
	tails_.push_back(newTail);
}

void Player::DeleteTails() { 
	tails_.pop_back(); 
}

void Player::AddBullets(PlayerBullet* bullet) { 
	// 弾をリストに追加
	bullets_.push_back(bullet); 
}

void Player::OnCollision() { AddTails(); }
