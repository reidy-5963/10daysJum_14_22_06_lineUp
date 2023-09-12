#pragma once
#include "BaseCharacter.h"
#include "BossBullet.h"
#include "BossFunnel.h"
#include "Input.h"

#include <list>
#include <optional>
#include <memory>

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
	/// 行動の管理
	/// </summary>
	void ActionControl();

	/// <summary>
	/// 弾のリスト取得
	/// </summary>
	/// <returns></returns>
	const std::list<BossBullet*>& GetBullets() { return bullets_; }

	/// <summary>
	/// ファンネルのリスト取得
	/// </summary>
	/// <returns></returns>
	const std::list<BossFunnel*>& GetFunnels() { return funnels_; }

private:	// 行動関数

	/// <summary>
	/// 突進
	/// </summary>
	void RushAttack();

	void RushAttackInitialize();

	void RushAttackSetup();

	/// <summary>
	/// 突進の準備
	/// </summary>
	void RushAlert();

	void RushAlertInitialize();

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
	/// ミサイルファンネル
	/// </summary>
	void FunnelAttack();

	void FunnelAttackInitialize();

	/// <summary>
	/// 十字・クロスショット
	/// </summary>
	void CrossAttack();

	void CrossAttackInitialize();

	/// <summary>
	/// 通常の動き
	/// </summary>
	void RootUpdate();

	void RootInitialize();

	void ActionTable();

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
	void GenerateBullet(Vector2& velocity, float speedValue);

	void GenerateFunnel(int type);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossEnemy();

	void RespownBoss();

	void SetHp(int hp) { hp_ = hp; }

	int GetHp() { return hp_; }

	bool IsDead() { return isDead_; }

	bool IsAlive() { return isAlive_; }

private:
	// 入力
	Input* input_ = nullptr;
	// スプライト
	std::unique_ptr<Sprite> rushSprite_;
	std::unique_ptr<Sprite> hpSprite_;
	std::unique_ptr<Sprite> hpShadowSprite_;

	// 弾用のテクスチャ
	uint32_t bulletTex_ = 0u;
	// ファンネル用のテクスチャ
	uint32_t funnelTex_ = 0u;
	// 突進先用のテクスチャ
	uint32_t rushPointTex_ = 0u;
	// HP用のテクスチャ
	uint32_t hpTex_ = 0u;
	uint32_t hpShadowTex_ = 0u;

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
	std::list<BossFunnel*> funnels_;
	
	bool isFunnelAttackNow_ = false;

	bool isAlive_ = false;

	bool isDead_ = false;

	int funnelCount_ = 0;

	const int SetMaxHp = 500;
	int hp_ = SetMaxHp;
	Vector2 hpGaugeSize{};

	//
	uint32_t particleTex = 0u;

private: // 突進
	// 補間レート
	float rushMove_t_ = 0;
	// 突進のカウント
	int rushCount_ = 0;
	// 突進までの時間
	int kRushTimer_ = 120;
	// 突進カウント用フラグ
	bool isRush_ = false;

	bool isRushNow_ = false;

	Vector2 ScPos = {};

private: // 全方位
	// 角度
	float rotateDegree = 0;

	float rotateRadian_ = 0;

	float rotate_t_ = 0;

private: // 行動管理用変数
	/// <summary>
	/// 行動のパターン
	/// </summary>
	enum class Behavior {
		kRoot,
		kRush,
		kGuided,
		kBarrage,
		kRushAlert,
		kFunnel,
		kCross,
	};
	enum PrevBehavior {
		kNowNone,
		kNowRush,
		kNowGuided,
		kNowBarrage,
		kNowFunnel,
	};

	std::list<Behavior> actions_;

	// 状態
	Behavior behavior_ = Behavior::kRoot;
	// 次の動きのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	
	/// <summary>
	/// モードカウント
	/// </summary>
	int modeCount_ = 0;
	int kModeEndTimer_ = 0;

private: // 動きの管理
	int actionTimer_ = 0;
	int kActionCoolTime_ = 60 * 5;

	void RandomActionManager();

	int ConvertSeconds(int second) { return int(second * 60); }

	float MaxHpSize = 0.0f;

	int isActionNow_ = false;

	bool isTest_ = false;

private: // 揺らす系
	Vector2 shakeVelo_{};
	bool isDamageShake = false;
	int amplitNum = 12;
	std::unique_ptr<ParticleManager> particle_ = nullptr;
	Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};

	float alphaOffset = 0.007f;
};
