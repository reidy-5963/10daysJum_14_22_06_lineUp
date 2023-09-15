#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>
#include <list>

#include "BaseScene.h"
#include "Player.h"
#include "Enemy.h"
#include "BossEnemy.h"
#include "Scroll.h"
#include "EnemyManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// グローバル変数の初期化処理
	/// </summary>
	void InitializeGrobalVariables();
	

	/// <summary>
	/// グローバル変数の更新処理
	/// </summary>
	void ApplyGrobalVariables();

	/// <summary>
	/// 当たり判定
	/// </summary>
	void CheckAllCollision();
	

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// 画面全体を揺らす、動かす時用
	Vector2 gameScenePos{};

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// エネミー
	EnemyManager* enemyManager_;

	// ボス
	std::unique_ptr<BossEnemy> boss_ = nullptr;

	// 雑魚敵を何体倒したらいいか
	int setKillCount = 30;

	// 雑魚敵の倒した数
	int killCount_ = 0;

	// ボスの沸き
	bool isBossRespown_ = false;
	
	// スクロールインスタンス
	Scroll* scroll_;

	// 画面の揺れ用
	Vector2 sceneShakevelo_{};
	bool issceneShake = false;
	int sceneaAmplitNum = 40;

#pragma region 背景
	// 背景スプライト関係
	std::unique_ptr<Sprite> back[4];

	uint32_t backTex;
	Vector2 backPos[4];
#pragma endregion

#pragma region 音 
	float volume = 0.02f;
	float SEvolume = 0.1f;

	uint32_t BGMHandle_ = 0u;
#pragma endregion

#pragma region 制限時間
	// 制限時間
	int setGameTime = 60 * 140;
	// 制限時間タイマー
	int gameTimer = setGameTime;
	// ゲーム終了フラグ
	bool isGameSet_ = false;

#pragma endregion

#pragma region 制限時間のスプライト関係
	// 制限時間スプライト
	std::unique_ptr<Sprite> timerNum[3];
	// 制限時間スプライトの位置(この位置をベースに)
	Vector2 TimerNumPos_{};
	// 数字一つ一つの位置
	Vector2 timerNumPos[3];
	// 数字テクスチャ
	uint32_t timerNumTex_;

#pragma endregion
	std::unique_ptr<Sprite> enemyNum[2];
	Vector2 enemyNumPos_{};
	Vector2 enemyNumPos[2];
	uint32_t enemyNumTex_;

	uint32_t pickUpTailSEHandle_;

	std::unique_ptr<Sprite> enemy_UI;
	Vector2 enemyUIPos_;
	uint32_t enemyUITex_;

	std::unique_ptr<Sprite> timer_UI;
	Vector2 timerUIPos_;
	uint32_t timerUITex_;

	std::unique_ptr<Sprite> enemy_UI2;
	Vector2 enemy_UI2Pos_;
	uint32_t enemy_UI2Tex_;

#ifdef _DEBUG
	int setTailTime = 3;
	int pickUpTailTime = 60 * setTailTime;
	int setFunnelDamage = 2;
	int funnelDamage = 60 * setTailTime;
	int setEneBulletDamage = 1;
	int eneBulletDamage = 60 * setEneBulletDamage;
	int setBossEnemyDamage = 3;
	int bossEnemyDamage = 60 * setBossEnemyDamage;
#endif // _DEBUG
};