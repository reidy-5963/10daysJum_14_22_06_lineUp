#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "TextureManager.h"
#include "WinApp.h"

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

	// スプライトの生成
	sprite_.reset(Sprite::Create(charaTex_, {720, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	// 初期位置(仮)
	pos_ = {760, 320};
	prePos_ = pos_;
	markerPos_ = pos_;
	preMarkerPos_ = markerPos_;
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
		prePos_ = pos_;
		// 線形補間の初期化
		move_t_ = 0.0f;
	}

	// 1.0になるまで加算
	if (move_t_ >= 1.0f) {
		move_t_ = 1.0f;
	} else {
		move_t_ += 0.01f;
	}

	// ベジエ曲線のスタート位置計算
	bezierStartPos_.x = MyMath::lerp(move_t_, prePos_.x, preMarkerPos_.x);
	bezierStartPos_.y = MyMath::lerp(move_t_, prePos_.y, preMarkerPos_.y);
	
	// ベジエ曲線の終わり位置計算
	bezierEndPos_.x = MyMath::lerp(move_t_, preMarkerPos_.x, markerPos_.x);
	bezierEndPos_.y = MyMath::lerp(move_t_, preMarkerPos_.y, markerPos_.y);

	// 実際にプレイヤーの位置を計算
	pos_.x = MyMath::lerp(move_t_, bezierStartPos_.x, bezierEndPos_.x);
	pos_.y = MyMath::lerp(move_t_, bezierStartPos_.y, bezierEndPos_.y);


	// スプライトに位置を反映させる
	sprite_->SetPosition(pos_);

	// 向きの計算
	Vector2 move = bezierEndPos_ - bezierStartPos_;
	sprite_->SetRotation(std::atan2(move.y, move.x));
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
	// スプライトの描画
	sprite_->Draw();
}
