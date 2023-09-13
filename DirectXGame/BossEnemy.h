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

	/// <summary>
	/// グローバル変数の初期化処理
	/// </summary>
	void InitializeGrobalVariables();

	/// <summary>
	/// グローバル変数の更新処理
	/// </summary>
	void ApplyGrobalVariables();

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

	bool IsParasite() { return isParasite_; }

	void BossDirection();

private:
	// 入力
	Input* input_ = nullptr;
	// スプライト
	std::unique_ptr<Sprite> rushSprite_;
	std::unique_ptr<Sprite> hpSprite_;
	std::unique_ptr<Sprite> hpShadowSprite_;
	std::unique_ptr<Sprite> directSprite_;
#pragma region テクスチャ
	// ラッシュ時の画像
	uint32_t bossRushTex_ = 0u;
	// ファンネル時の画像
	uint32_t bossFunnelTex_ = 0u;
	// 何もしてないときの画像
	uint32_t bossNoneActionTex_ = 0u;
	// 弾用のテクスチャ
	uint32_t bulletTex_ = 0u;
	// ファンネル用のテクスチャ
	uint32_t funnelTex_ = 0u;
	// 突進先用のテクスチャ
	uint32_t rushPointTex_ = 0u;
	// HP用のテクスチャ
	uint32_t hpTex_ = 0u;
	uint32_t hpShadowTex_ = 0u;
	// ボスの方向の矢印
	uint32_t directArrowTex_ = 0u;
#pragma endregion

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

private:
	// ファンネルのSE
	uint32_t funnelSEHandle_ = 0;
	// 突進のSE
	uint32_t rushSEHandle_ = 0;
	// 突進の出現
	uint32_t rushSpawnSEHandle_ = 0;
	// 突進の警告
	uint32_t rushAlertSEHandle_ = 0;
	// ボスの弾の音
	uint32_t bulletSEHandle_ = 0;
	bool isRushSound_ = 0;

	// 画面外の方向
	bool isScreenOut_ = false;
	Vector2 directArrowPos_ = {};
	Vector2 ScDirect_ = {};

private:
	// 弾のリスト
	std::list<BossBullet*> bullets_;
	// ミサイルファンネル
	std::list<BossFunnel*> funnels_;
	// ファンネル攻撃中か
	bool isFunnelAttackNow_ = false;
	// 生きているか
	bool isAlive_ = false;
	// 死んだか
	bool isDead_ = false;
	// ファンネルの行動回数
	int funnelCount_ = 0;
	// ボスのHp関係
	int SetMaxHp = 300;
	int hp_ = SetMaxHp;
	Vector2 hpGaugeSize{};
	float MaxHpSize = 0.0f;

	int prevAction_ = 0;

	int behaviorRand_ = 0;

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
	// 突進中の確認フラグ
	bool isRushNow_ = false;

	Vector2 ScPos = {};

private: // 全方位
	// 角度
	float rotateDegree = 0;
	// 角度ラジアン
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
	// 行動のリクエストリスト
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
	// 行動の間隔
	int kActionCoolTime_ = 60 * 5;
	// ランダムアクション用
	//void RandomActionManager();
	// 秒数に変換の関数
	int ConvertSeconds(int second) { return int(second * 60); }
	// アクション中か
	int isActionNow_ = false;
	// テーブルのラスト
	bool isLastAction_ = false;
public:
	// 最後の行動かの取得
	bool GetIsLastAction() { return isLastAction_; }
	// 最後の行動かの設定
	void SetIsLastAction(bool flag) { isLastAction_ = flag; }

private: // 揺らす系
	Vector2 shakeVelo_{};
	bool isDamageShake = false;
	int amplitNum = 12;
	std::unique_ptr<ParticleManager> particle_ = nullptr;
	Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};

	float alphaOffset = 0.007f;
	bool isParasite_ = false;
	int collapseAniTimer = 0;
	int collapseAniNumber = 0;
	int collapseAniScene = 5;
	int collapseAnioneTime = 20;

	uint32_t parasiteTex_;
};
