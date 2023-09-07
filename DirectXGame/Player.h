#pragma once
#include "BaseCharacter.h"
#include "PlayerBullet.h"
#include "Tail.h"
#include <list>


/// <summary>
/// プレイヤー
/// </summary>
class Player : public BaseCharacter {
public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// キー入力での移動処理
	/// </summary>
	void KeyMove();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	void OnCollision() override;
	void CursorUpdate();

	/// <summary>
	/// 線形補間用のtのカウント
	/// </summary>
	/// <param name="t">t</param>
	/// <param name="endT">1.0fになったあとの値の初期化</param>
	/// <param name="flag">フラグ</param>
	/// <param name="setFlag">true or false</param>
	/// <param name="offset">増えてく値</param>
	void CountT(float& t, const float endT, bool& flag, const bool setFlag, float offset);

	void MarkerControl();

	void LeftClickUpdate();

public: // 追加関数
	/// <summary>
	/// 尻尾の追加
	/// </summary>
	void AddTails();

	/// <summary>
	/// 尻尾の追加
	/// </summary>
	void DeleteTails();

	/// <summary>
	/// 弾の追加
	/// </summary>
	/// <param name="bullet">追加する弾</param>
	void AddBullets(PlayerBullet* bullet);

public: // ゲッター
	/// <summary>
	/// 弾テクスチャの取得
	/// </summary>
	/// <returns>弾テクスチャ</returns>
	uint32_t GetBulletTex() { return bulletTexture_; }

	/// <summary>
	/// マーカーの位置の取得
	/// </summary>
	/// <returns></returns>
	Vector2 GetMarkerPos() { return markerPos_; }

	/// <summary>
	/// 弾のリストの取得
	/// </summary>
	/// <returns></returns>
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

private: // メンバ変数
	// 入力
	Input* input_ = nullptr;
	// マウス位置
	POINT mousePos;

#pragma region マーカー
	// マーカーのスプライト
	std::unique_ptr<Sprite> markerSprite_ = nullptr;
	// マーカーのテクスチャ
	uint32_t markerTex_ = 0u;
	// マーカーの位置
	Vector2 markerPos_{};
	// (前クリックした時のマーカーの位置)
	Vector2 preMarkerPos_{};
#pragma endregion

#pragma region 通常時の動き用
	// 回転するための4つ目の点
	Vector2 RotateRootPos_;
	// 回転用の線形補完
	float root_t_ = 0.0f;
	// 回転用のフラグ
	bool isRootMove_ = false;
#pragma endregion

	// 前フレーム時の位置
	Vector2 clickPlayerPos_{};

	// 前フレーム時の位置
	Vector2 prePos_{};

	// 動くときの線形補完
	float move_t_ = 0.0f;

	// ベジエで動くときのスタート位置
	Vector2 bezierStartPos_{};
	// ベジエで動くときの終わり位置//
	Vector2 bezierEndPos_{};

	// 弾
	std::list<PlayerBullet*> bullets_;
	// 弾テクスチャ
	uint32_t bulletTexture_ = 0u;

	const float kDeadOffset = 740.0f;

	// 尻尾
	std::list<Tail*> tails_;

	// 尻尾テクスチャ
	uint32_t tailTexture_ = 0u;

	// 尻尾の数
	const int kMaxTail_ = 6;

	//
	float BulletRadian = 0.1f;
	Vector2 markerAddRadian[2];
	Vector2 PlayerAddRadian[2];
	float radianOffset = 1.0f;

	bool isMtM1 = false;
	bool isM1tM2 = false;
	bool isM2tP = false;
	bool isPtP1 = false;
	bool isP1tP2 = false;
	bool isP2tM = false;

	// test
	std::unique_ptr<Sprite> m1 = nullptr;
	std::unique_ptr<Sprite> m2 = nullptr;
	std::unique_ptr<Sprite> p1 = nullptr;
	std::unique_ptr<Sprite> p2 = nullptr;
	std::unique_ptr<Sprite> origin_ = nullptr;
	Vector2 originPos_{};
	//
};