#include "Tail.h"
#include "MyMath.h"
#include "ImGuiManager.h"

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
		pos_.x = MyMath::lerp(t_, prePos_.x, velo_.x);
		pos_.y = MyMath::lerp(t_, prePos_.y, velo_.y);
	}
	// もし動いていないとき
	if (!isMove_) {
		// 動いてなかったときの位置を取得
		prePos_ = pos_;
		// 媒介変数tの初期化
		t_ = 0.0f;
		// 線形補完の完了地点を親の位置に
		velo_.x = parentPos_->x;
		velo_.y = parentPos_->y;

	}

	// 位置の更新処理
	sprite_->SetPosition(pos_);

	ImGui::Begin("debug");
	ImGui::Text("%f, %f", prePos_.x, prePos_.y);
	ImGui::Text("%f, %f", parentPos_->x, parentPos_->y);
	ImGui::Text("%f", t_);
	ImGui::End();

}

void Tail::Draw() { 
	//
	sprite_->Draw(); 
}
