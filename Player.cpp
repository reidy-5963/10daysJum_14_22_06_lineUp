#include "Player.h"
#include "TextureManager.h"

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
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update() {
	// 移動距離
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


	// 後でノーマライズするとこ
	// 移動速度をかけて加算する
	pos_.x += move.x * kMoveSpeed;
	pos_.y += move.y * kMoveSpeed;


	// スプライトに位置を反映させる
	sprite_->SetPosition(pos_);
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw() { 
	// スプライトの描画
	sprite_->Draw(); 
}
