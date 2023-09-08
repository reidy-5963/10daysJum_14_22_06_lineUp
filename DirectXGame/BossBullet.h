#pragma once
#include "BaseBullet.h"

class BossBullet : public BaseBullet
{
public: // メンバ関数
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(uint32_t texture, const Vector2& pos, Vector2 velosity) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	void OnCollision() override;

public:

	static Vector2 BulletDirection(Vector2& BossPos, Vector2& PlayerPos);

	void BulletDirectRotate();

public: // ファンネル用関数

	void FunnelInitialize(int type, Vector2& startPos,Vector2& endPos);

	void FunnelUpdate();

public:
	enum FunnelType {
		kNone,
		kHorizontal,
		kVertical,
	};

private: // ファンネル用変数

	float move_t_ = 0;

	int moveType_ = kNone;

	int turnCount_ = 0;

	int turnCoolTime_ = 0;

	bool isMove_ = false;

	Vector2 prevPos_ = {};

	Vector2 endPos_ = {};

};
