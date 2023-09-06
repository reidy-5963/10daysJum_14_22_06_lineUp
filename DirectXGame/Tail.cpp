#include "Tail.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include <cmath>
#include "Player.h"


/// <summary>
/// 初期化処理
/// </summary>
/// <param name="texture">テクスチャ</param>
/// <param name="parent">親になるスプライトの位置</param>
/// <param name="tailNo">尻尾番号</param>
void Tail::Initialize(uint32_t texture,const Vector2* parent, int tailNo) {

	// 親の座標	
	parentPos_ = parent;

	// 尻尾番号
	tailNo_ = tailNo;

	// スプライトの生成
	sprite_.reset(Sprite::Create(texture, {720, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	// 親の位置に初期化
	pos_.x = parentPos_->x;
	pos_.y = parentPos_->y;

	// 親の位置に初期化
	prePos_ = pos_; 
	
	// 線形補完の完了地点を親の位置に
	lerpEndPos_.x = parentPos_->x;
	lerpEndPos_.y = parentPos_->y;
}

void Tail::Update() {
	// もしプレイヤーが動いていたら
	if (isPlayerMove_) {
		isMove_ = true;
	}
	// もし動いているとき
	if (isMove_) {
		// 媒介変数t(0.0f ~ 1.0f)
		if (t_ > 1.0f) {
			t_ = 1.0f;
			isMove_ = false;
		} else {
			t_ += 0.1f;
		}

		// 線形補完
		pos_ = MyMath::lerp(t_, prePos_, lerpEndPos_);
	}
	// もし動いていないとき
	if (!isMove_) {
		// 動いてなかったときの位置を取得
		prePos_ = pos_;
		// 媒介変数tの初期化
		t_ = 0.0f;
		// 線形補完の完了地点を親の位置に
		lerpEndPos_.x = parentPos_->x;
		lerpEndPos_.y = parentPos_->y;

	}

	direction_.x = parentPos_->x - pos_.x;
	direction_.y = parentPos_->y - pos_.y;

	direction_ = MyMath::Normalize(direction_);
	// 位置の更新処理
	sprite_->SetPosition(pos_);
	sprite_->SetRotation(std::atan2(direction_.y, direction_.x));

	ImGui::Begin("debug");
	ImGui::Text("%f, %f", prePos_.x, prePos_.y);
	ImGui::Text("%f, %f", parentPos_->x, parentPos_->y);
	ImGui::Text("%d", bulletTimer_);
	ImGui::End();
	Fire();
}

void Tail::Draw() {
	//
	sprite_->Draw();
}

void Tail::Fire() {
	if (bulletTimer_ < 0) {
		bulletTimer_ = 0;
	} else {
		bulletTimer_--;
	}



	if (isFire_) {
		//** 尻尾の進む向きから弾を打ち出す向きを計算 **//

		// 実際に加算する値
		Vector2 move = {0.0f, -1.0f};
		// 進行方向をもとに回転行列の生成
		Matrix3x3 rotateMat = MyMath::MakeRotateMatrix(std::atan2(direction_.y, direction_.x));
		// 実際に動く値で平行移動行列を生成
		Matrix3x3 moveMat = MyMath::MakeTranslateMatrix(move);
		// 回転行列と平行移動行列を合成
		moveMat = MyMath::Multiply(moveMat, rotateMat);
		// 合成した行列から移動成分のみ取得
		move = {moveMat.m[2][0], moveMat.m[2][1]};

		//*****************************************//

		// 弾の生成
		PlayerBullet* newBullet = new PlayerBullet();
		// 弾の初期化処理
		newBullet->Initialize(player_->GetBulletTex(), *GetPosition(), move);
		// 弾をリストに追加
		player_->AddBullets(newBullet);
		// フラグを無効に
		isFire_ = false;
	}
	// もしフラグが無効なら
	else if (!isFire_) {
		// 尻尾とマーカーの当たり判定
		Vector2 Tail2Marker_distance;
		// 尻尾からマーカーまでのベクトル
		Tail2Marker_distance.x = player_->GetMarkerPos().x - pos_.x;
		Tail2Marker_distance.y = player_->GetMarkerPos().y - pos_.y;

		// 当たり判定の計算
		float distance =
		    std::sqrtf(std::powf(Tail2Marker_distance.x, 2) + std::powf(Tail2Marker_distance.y, 2));
		float radius = (32 + GetRadius());

		// もし尻尾とマーカーが当たっていれば
		if (distance <= radius && bulletTimer_ <= 0) {
			//攻撃フラグを有効
			SetIsFire(true);
			// タイマーセット
			bulletTimer_ = kBulletTime;

		}

	}
}
