#include "Tail.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "Player.h"
#include <cmath>

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="texture">テクスチャ</param>
/// <param name="parent">親になるスプライトの位置</param>
/// <param name="tailNo">尻尾番号</param>
void Tail::Initialize(uint32_t texture, const Vector2* parent, int tailNo, const bool* ParentBool) {

	// 親の座標
	parentPos_ = parent;
	parentBool_ = ParentBool;

	// 尻尾番号
	tailNo_ = tailNo;

	// 親の位置に初期化
	pos_.x = parentPos_->x;
	pos_.y = parentPos_->y;

	// 親の位置に初期化
	prePos_ = pos_;

	// 線形補完の完了地点を親の位置に
	lerpEndPos_.x = parentPos_->x;
	lerpEndPos_.y = parentPos_->y;

	// スプライトの生成
	sprite_.reset(Sprite::Create(texture, {-10.0f, -10.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

}

void Tail::Update() {
	if (!isHitOut_) {
		// もしプレイヤーが動いていたら
		if (isPlayerMove_) {
			isMove_ = true;
		}

		// 尻尾の動き更新処理
		//
		
		MoveUpdate();

		// 進行方向の更新処理
		DirectionUpdate();
	} 
	//
	else if (isHitOut_) {
		//Vector2 offset = {4.0f, 0.0f};

		//Vector2 P2T;
		//P2T.x = parentPos_->x - pos_.x;
		//P2T.y = parentPos_->y - pos_.y;

		//Matrix3x3 rotateMat = MyMath::MakeRotateMatrix(std::atan2(P2T.y, P2T.x));

		//offset = MyMath::TransformNormal(offset, rotateMat);

		//pos_.y += offset.y;
		//pos_.x += offset.x;
	}

	// 位置の更新処理
	BaseCharacter::Update();
	sprite_->SetRotation(std::atan2(direction_.y, direction_.x));
	
	// 弾の発射処理
	Fire();

#ifdef _DEBUG
	ImGui::Begin("debug");
	ImGui::Text("%f, %f", prePos_.x, prePos_.y);
	ImGui::Text("%f, %f", parentPos_->x, parentPos_->y);
	ImGui::Text("%d", bulletTimer_);
	ImGui::End();
#endif
}

void Tail::Draw() {
	// 尻尾の描画
	sprite_->Draw();
}

void Tail::OnCollision() { 
	if (!isHitOut_) {
		isHitOut_ = true; 
		tailNo_ = -1;
	}
	else if (isHitOut_) {
		//isDead_ = true;
	}
}

void Tail::Fire() {
	// タイマーカウント
	BulletTimerCount();

	// もしフラグが有効なら
	if (isFire_) {
		if (tailNo_ == 0) {
			//** 尻尾の進む向きから弾を打ち出す向きを計算 **//

			// 実際に加算する値
			Vector2 move = {0.0f, -1.0f};

			// 尻尾の進行方向から弾の撃つ向きを計算
			move = BulletDirectionInitialize(move);

			//*****************************************//
			
			// 弾の生成
			PlayerBullet* newBullet = new PlayerBullet();
			// 弾の初期化処理
			newBullet->Initialize(player_->GetBulletTex(), GetPosition(), move);
			// 弾をリストに追加
			player_->AddBullets(newBullet);
			// フラグを無効に
			isFire_ = false;
		}
		//
		else if (tailNo_ != 0) {
			//** 尻尾の進む向きから弾を打ち出す向きを計算 **//

			// 実際に加算する値
			float fireBulletRad = std::atan2(direction_.y, direction_.x);
			float minrad = fireBulletRad - offsetRadian / 2;
			float tamanoaida = offsetRadian / float(tailNo_ + 2);

			for (int i = 0; i < tailNo_ + 1; i++) {
				Vector2 move = {0.0f, -1.0f};

				// 尻尾の進行方向から弾の撃つ向きを計算

				Matrix3x3 rotateMat =
				    MyMath::MakeRotateMatrix(minrad + (float(i) * tamanoaida));
				// 実際に動く値で平行移動行列を生成
				Matrix3x3 moveMat = MyMath::MakeTranslateMatrix(move);
				// 回転行列と平行移動行列を合成
				//rotateTailMat = MyMath::Multiply(rotateTailMat, rotateMat);
				moveMat = MyMath::Multiply(moveMat, rotateMat);
				// 合成した行列から移動成分のみ取得
				move = {moveMat.m[2][0], moveMat.m[2][1]};

				
				// 弾の生成
				PlayerBullet* newBullet = new PlayerBullet();
				// 弾の初期化処理
				newBullet->Initialize(player_->GetBulletTex(), GetPosition(), move);
				// 弾をリストに追加
				player_->AddBullets(newBullet);
			}

			//*****************************************//

			// フラグを無効に
			isFire_ = false;
		}


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
		if (tailNo_ == 0) {
			if (distance <= radius && bulletTimer_ <= 0 && !parentBool_) {
				// 攻撃フラグを有効
				SetIsFire(true);
				// タイマーセット
				bulletTimer_ = kBulletTime;
			}
		} 
		else if (tailNo_ != 0) {
			if (distance <= radius && bulletTimer_ <= 0 && parentBool_) {
				// 攻撃フラグを有効
				SetIsFire(true);
				// タイマーセット
				bulletTimer_ = kBulletTime;
			}

		}
	}
}

void Tail::BulletTimerCount() {
	// 0より小さければ0に固定. そうでなければ減り続けてる
	if (bulletTimer_ < 0) {
		bulletTimer_ = 0;
	} else {
		bulletTimer_--;
	}
}

void Tail::DirectionUpdate() {
	// 進行方向を自分の位置->親の位置に
	direction_.x = parentPos_->x - pos_.x;
	direction_.y = parentPos_->y - pos_.y;

	// 単位化
	direction_ = MyMath::Normalize(direction_);
}

void Tail::MoveUpdate() {
	// もし動いているとき
	if (isMove_) {
		// 媒介変数t(0.0f ~ 1.0f)
		if (t_ >= 1.0f) {
			t_ = 1.0f;
			isMove_ = false;
		} else {
			t_ += 0.2f;
		}

		// 線形補完
		//pos_ = MyMath::CatmullRom(prePos_, pos_, lerpEndPos_, lerpEndPos_, t_);
		if (!isMove_) {
		
		}

		pos_ = MyMath::lerp(t_, prePos_, lerpEndPos_);
	}
	// もし動いていないとき
	if (!isMove_) { 
		// 線形補完の完了地点を親の位置に
		lerpEndPos_.x = parentPos_->x;
		lerpEndPos_.y = parentPos_->y;

		// 動いてなかったときの位置を取得
		prePos_ = pos_;
		// 媒介変数tの初期化
		t_ = 0.0f;
		isMove_ = true;
	}
}

Vector2 Tail::BulletDirectionInitialize(Vector2 move) {
	// 実際に加算する値
	Vector2 result;
	// 進行方向をもとに回転行列の生成
	Matrix3x3 rotateMat = MyMath::MakeRotateMatrix(std::atan2(direction_.y, direction_.x));
	// 実際に動く値で平行移動行列を生成
	Matrix3x3 moveMat = MyMath::MakeTranslateMatrix(move);
	// 回転行列と平行移動行列を合成
	moveMat = MyMath::Multiply(moveMat, rotateMat);
	// 合成した行列から移動成分のみ取得
	result = {moveMat.m[2][0], moveMat.m[2][1]};

	return result;
}
