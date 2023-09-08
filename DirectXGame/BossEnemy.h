#pragma once
#include "BaseCharacter.h"
#include "BossBullet.h"
#include "Input.h"

#include <list>
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

	/// <summary>
	/// 弾の更新処理
	/// </summary>
	void BulletUpdate();

	/// <summary>
	/// 弾のリスト取得
	/// </summary>
	/// <returns></returns>
	const std::list<BossBullet*>& GetBullets() { return bullets_; }

private:	// 行動関数

	/// <summary>
	/// 突進
	/// </summary>
	void RushAttack();

	void RushAttackInitialize();

	/// <summary>
	/// 誘導弾
	/// </summary>
	void GuidedAttack();

	void GuidedAttackInitialize();

	/// <summary>
	/// 全方位弾
	/// </summary>
	void BarrageAttack();

	void BarrageAttackInitialize();

	/// <summary>
	/// ビーム弾
	/// </summary>
	void BeamAttack();

	void BeamAttackInitialize();

	/// <summary>
	/// ミサイルファンネル
	/// </summary>
	void FunnelAttack();

	void FunnelAttackInitialize();

	/// <summary>
	/// 通常の動き
	/// </summary>
	void RootUpdate();

	void RootInitialize();

public:	// 設定・取得
	/// <summary>
	/// プレイヤーの座標を設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPlayer(Vector2 pos) { nowPlayerPos_ = pos; }

	/// <summary>
	/// 弾の追加
	/// </summary>
	/// <param name="bullet"></param>
	void GenerateBullet(Vector2& velocity);

	void GenerateFunnel(Vector2& velocity, int type);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossEnemy();

private:
	// 入力
	Input* input_ = nullptr;
	// 弾用のテクスチャ
	uint32_t bulletTex_ = 0u;
	// ファンネル用のテクスチャ
	uint32_t funnelTex_ = 0u;
	// 毎フレームプレイヤーの座標補間変数
	Vector2 nowPlayerPos_ = {};
	// 必要時のプレイヤーの座標変数
	Vector2 prevPlayerPos_ = {};
	// 開始時の座標
	Vector2 prevBossPos_ = {};

	// 弾のサイズ減少値
	float decreaseValue_ = 0.1f;
	// 消えるサイズ
	float deadZone_ = 5.0f;
	// イージングを判定する距離
	float easeInRange_ = 1000.0f;

	// 弾のリスト
	std::list<BossBullet*> bullets_;
	// ミサイルファンネル
	std::list<BossBullet*> finFunnel_;
	

private: // 突進
	// 補間レート
	float rushMove_t_ = 0;
	// 突進のカウント
	int rushCount_ = 0;
	// 突進までの時間
	int kRushTimer_ = 60;
	// 突進カウント用フラグ
	bool rushFlag_ = false;

private: // 全方位
	// 角度
	float rotateDegree = 0;

private: // 行動管理用変数
	/// <summary>
	/// 行動のパターン
	/// </summary>
	enum class Behavior {
		kRoot,
		kRush,
		kGuided,
		kBarrage,
		kBeam,
		kFunnel,
	};
	// 状態
	Behavior behavior_ = Behavior::kRoot;
	// 次の動きのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	
	/// <summary>
	/// モードカウント
	/// </summary>
	int modeCount_ = 0;
	int kModeEndTimer_ = 0;
};
