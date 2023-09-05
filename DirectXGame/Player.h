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

	void AddTails();
	void AddBullets(PlayerBullet* bullet);
	uint32_t GetBulletTex() { return bulletTexture_; }
	Vector2 GetMarkerPos() { return markerPos_; }
	/// <summary>
	/// 衝突処理
	/// </summary>
	void OnCollision() override;

private: // メンバ変数	
	// 入力
	Input* input_ = nullptr;
	// マウス位置
	POINT mousePos;
	
	// マーカー(前フレーム分も取得)
	Vector2 markerPos_{};
	Vector2 preMarkerPos_{};

	Vector2 RotateRootPos_;
	float root_t_ = 0.0f;
	bool isRootMove_ = false;

	// 前フレーム時の位置
	Vector2 clickPlayerPos_{};

	// 前フレーム時の位置
	Vector2 prePos_{};

	// 
	float move_t_ = 0.0f;

	// 
	Vector2 bezierStartPos_{};
	// 
	Vector2 bezierEndPos_{};

	//
	std::list<PlayerBullet*> bullets_;
	
	std::list<Tail*> tails_;

	const int setBulletTime = 60;
	int bulletTimer_ = setBulletTime;
	bool isFire = false;

	uint32_t tailTexture_ = 0u;
	uint32_t bulletTexture_ = 0u;

	uint32_t markerTex_ = 0u;
	std::unique_ptr<Sprite> markerSprite_ = nullptr;
};