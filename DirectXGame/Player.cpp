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

		Vector2 distance;
		distance = markerPos_ - pos_;

		const float kRootSpeed = 450.0f;
		RotateRootPos_[0] = markerPos_;
		if (distance.x > 0 && distance.y > 0) {
			RotateRootPos_[1].x = markerPos_.x;
			RotateRootPos_[2].x = markerPos_.x + kRootSpeed;
			RotateRootPos_[3].x = markerPos_.x + kRootSpeed;

			RotateRootPos_[1].y = markerPos_.y - kRootSpeed;
			RotateRootPos_[2].y = markerPos_.y;
			RotateRootPos_[3].y = markerPos_.y - kRootSpeed;
		}
		else if (distance.x < 0 && distance.y > 0) {
			RotateRootPos_[1].x = markerPos_.x;
			RotateRootPos_[2].x = markerPos_.x - kRootSpeed;
			RotateRootPos_[3].x = markerPos_.x - kRootSpeed;

			RotateRootPos_[1].y = markerPos_.y;
			RotateRootPos_[2].y = markerPos_.y + kRootSpeed;
			RotateRootPos_[3].y = markerPos_.y + kRootSpeed;
		} 
		else if (distance.x > 0 && distance.y < 0) {
			RotateRootPos_[1].x = markerPos_.x - kRootSpeed;
			RotateRootPos_[2].x = markerPos_.x;
			RotateRootPos_[3].x = markerPos_.x - kRootSpeed;

			RotateRootPos_[1].y = markerPos_.y;
			RotateRootPos_[2].y = markerPos_.y - kRootSpeed;
			RotateRootPos_[3].y = markerPos_.y - kRootSpeed;
		} 
		else if (distance.x < 0 && distance.y < 0) {
			RotateRootPos_[1].x = markerPos_.x;
			RotateRootPos_[2].x = markerPos_.x - kRootSpeed;
			RotateRootPos_[3].x = markerPos_.x - kRootSpeed;

			RotateRootPos_[1].y = markerPos_.y;
			RotateRootPos_[2].y = markerPos_.y + kRootSpeed;
			RotateRootPos_[3].y = markerPos_.y + kRootSpeed;
		}

		
		

	} 
	
	
	if (!isMove_) {
		if (!isRootMove_) {

			// ベジエ曲線のスタート位置計算
			bezierStartPos_ = MyMath::lerp(root_t_, RotateRootPos_[0], RotateRootPos_[2]);

			// ベジエ曲線の終わり位置計算
			bezierEndPos_ = MyMath::lerp(root_t_, RotateRootPos_[2], RotateRootPos_[3]);

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
			bezierStartPos_ = MyMath::lerp(root_t_, RotateRootPos_[3], RotateRootPos_[1]);

			// ベジエ曲線の終わり位置計算
			bezierEndPos_ = MyMath::lerp(root_t_, RotateRootPos_[1], RotateRootPos_[0]);
			
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

	if (input_->TriggerKey(DIK_SPACE)) {
		AddTails();
	}

	for (Tail* tail : tails_) {

		tail->Update();
	}

	Fire();

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	ImGui::Begin("debug");
	ImGui::Text("%f", root_t_);
	ImGui::Text("0 : %f, %f", RotateRootPos_[0].x, RotateRootPos_[0].y);
	ImGui::Text("1 : %f, %f", RotateRootPos_[1].x, RotateRootPos_[1].y);
	ImGui::Text("2 : %f, %f", RotateRootPos_[2].x, RotateRootPos_[2].y);
	ImGui::Text("3 : %f, %f", RotateRootPos_[3].x, RotateRootPos_[3].y);

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

void Player::Fire() {

}

void Player::AddTails() {
	Tail* newTail = new Tail();

	if (tails_.size() != 0) {
		newTail->Initialize(
		    tailTexture_, tails_.back()->GetPosition(), (tails_.back()->GetTailNo() + 1));
	} else {
		newTail->Initialize(tailTexture_, &pos_, 0);
	}
	
	newTail->SetPlayer(this);
	tails_.push_back(newTail);
}

void Player::AddBullets(PlayerBullet* bullet) { bullets_.push_back(bullet); }

void Player::OnCollision() 
{ 

}
