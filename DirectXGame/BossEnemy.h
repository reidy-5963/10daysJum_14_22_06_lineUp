#pragma once
#include "BaseCharacter.h"
#include "Input.h"
#include <optional>

class BossEnemy : public BaseCharacter
{
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

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

public:	// 行動関数

	void RushAttack();

	void RushAttackInitialize();

	void GuidedAttack();

	void GuidedAttackInitialize();

	void BeamAttack();

	void BeamAttackInitialize();

public:	// 設定・取得
	/// <summary>
	/// プレイヤーの座標を設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPlayer(Vector2 pos) { nowPlayerPos_ = pos; }

private:
	Input* input_ = nullptr;

	// 毎フレームプレイヤーの座標補間変数
	Vector2 nowPlayerPos_ = {};
	// 必要時のプレイヤーの座標変数
	Vector2 prevPlayerPos_ = {};
	// 開始時の座標
	Vector2 prevBossPos_ = {};

	float rushMove_t_ = 0;

	int rushCount_ = 0;

	int kRushTimer_ = 60;

	bool rushFlag_ = false;

	/// <summary>
	/// 行動のパターン
	/// </summary>
	enum class Behavior {
		kRoot,
		kRush,
		kGuided,
		kBarrage,
		kBeam,
	};

	// 状態
	Behavior behavior_ = Behavior::kRoot;

	// 次の動きのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

};
