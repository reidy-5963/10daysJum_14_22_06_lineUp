#pragma once
#include "BaseBullet.h"
#include "ParticleManager.h"

class BossFunnel : public BaseBullet
{
public: // メンバ関数
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(uint32_t texture, int type, Vector2& startPos, Vector2& endPos);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Vector2& playerPos);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	void OnCollision() override;

	void SetParticleTex(uint32_t tex) { particleTex_ = tex; }

public:
	enum FunnelType {
		kNone,
		kHorizontal,
		kVertical,
	};

private: // ファンネル用変数
	float move_t_ = 0;
	// 方向
	int moveType_ = kNone;
	// ターン回数
	int turnCount_ = 0;
	// ターンまでのクールタイム計測
	int turnCoolTime_ = 0;
	// ターンまでの時間
	int kTurnCoolTime = 45;

	// 移動しているか
	bool isMove_ = false;
	// スタート座標
	Vector2 prevPos_ = {};
	// エンド座標
	Vector2 endPos_ = {};
	// 進む方向ベクトル
	Vector2 moveDirect_ = {};
	// 直進時の速さ変数
	float kMoveSpeed_ = 7.5f;
	float kInitSpeed = 15.0f;

	std::unique_ptr<ParticleManager> particle_ = nullptr;
	uint32_t particleTex_ = 0u;
	Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};

	bool isSENow_ = false;

	// SE用のハンドル
	uint32_t SEHandle_ = 0;

public:

	void SetSE(int Handle) { SEHandle_ = Handle; }
};
