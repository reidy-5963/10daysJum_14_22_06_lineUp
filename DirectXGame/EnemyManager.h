#pragma once
#include "Vector2.h"
#include "Enemy.h"
#include<list>
#include "Input.h"
#include <memory>


class EnemyManager 
{
public:
	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static EnemyManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	void SetSceneVelo(Vector2 velo) { sceneVelo_ = velo; }

	void EnemyUpdate();

public:	// 設定、取得

	/// <summary>
	/// 弾のリストの取得
	/// </summary>
	/// <returns></returns>
	const std::list<Enemy*>& GetEnemyLists() { return enemys_; }

	/// <summary>
	/// プレイヤーの位置取得
	/// </summary>
	/// <param name="pos"></param>
	void SetPlayer(Vector2 pos) { playerPos_ = pos; }

	void SetBoss(Vector2 pos) { bossPos_ = pos; }

	//BossEnemy* GetBossEnemy() { return boss_; }

public: // 沸きパターン
	/// <summary>
	/// 画面外から中央に
	/// </summary>
	void DiagonalBehavior();

	/// <summary>
	/// 画面内からそれぞれの方向に
	/// </summary>
	void DiagonalClockWiseBehavior();

	/// <summary>
	/// 矢印の出現
	/// </summary>
	/// <param name="switchPatt"></param>
	void ArrowBehavior(int switchPatt);

	/// <summary>
	/// 矢印隊列の操作
	/// </summary>
	void ArrowBehaviorControl();

	void ArrowBehaviorPlay();

	void VerticalSpawn();

	void HorizontalSpawn();


public:	// リスポーンタイプ
	/// <summary>
	/// 適当湧き
	/// </summary>
	void FormationSpawnUpdate();

	Vector2 RandomRadianVector();

	void FourPointsSpawn();

	/// <summary>
	/// 突進の着地時にスポーンさせる関数
	/// </summary>
	void RushSpawn();

	void StartSpawn();

public: // 敵の生成関数
	// 四隅
	void CreateEnemy(int spownPoint);
	
	/// <summary>
	/// 基本の生成
	/// </summary>
	/// <param name="position"></param>
	/// <param name="velocity"></param>
	void AddEnemy(const Vector2& position, const Vector2& velocity);

	/// <summary>
	/// 向きの指定可能
	/// </summary>
	/// <param name="position"></param>
	/// <param name="velocity"></param>
	void AddEnemy(const Vector2& position, const Vector2& velocity, const Vector2& direction);

	void SetIsBossAlive(bool isAlive) { isBossAlive_ = isAlive; }

private:

	Input* input_ = nullptr;

	// 共通テクスチャ
	uint32_t charaTex_ = 0u;
	uint32_t parasiteTex_ = 0u;
	uint32_t collapseTex_ = 0u;
	uint32_t particleTex_ = 0u;

	// エネミー
	std::list<Enemy*> enemys_;
	// プレイヤーの座標の入れ物
	Vector2 playerPos_ = {};
	// ボスの座標の入れ子
	Vector2 bossPos_ = {};
	// シーンのシェイク
	Vector2 sceneVelo_{};

	int ArrowCoolTime = 0;
	// 矢印のフラグ
	bool isArrowRespown_ = false;

	int kArrowDelay_ = 120;

	bool isBossAlive_ = false;

private:
	// パターン湧きしているか
	bool isPatternNow_ = false;
	// パターンの時間
	int patternInterval_ = 0;
	// 湧き間隔の秒数
	int kIntervalSecond_ = 10;
	// パターン湧きの間隔
	int kInterval_ = 60 * kIntervalSecond_;

private:	// Arrowの変数
	enum Pattern 
	{
		kDown,
		kUp,
		kLeft,
		kRight,
	};

	Vector2 plPrevPos_ = {};

private: // ステージでの管理系
	int autoSpawnSecond_ = 4;
	int kRespownTimer_ = 60 * autoSpawnSecond_;
	int normalSpawnTimer_ = 0;
	int spawnShiftFrame_ = 0;
	
	// リスポーンの座標
	Vector2 resCircle_ = {};
	float resCircleRadius_ = 0;

	enum SpawnPoint {
		kLeftTop,
		kLeftBottom,
		kRightTop,
		kRightBottom,
	};

	// マップ全体のサイズ
	Vector2 screenSize_ = {1920 * 2, 1080 * 2};

private: // 全体マップを縦横3分割ごと
	float scWidthHalfSize = (float)1280 / 2;
	float scHeightHalfSize = (float)720 / 2;
	Vector2 areaLeft2Top = {screenSize_.x * (1.0f / 3.0f), 720.0f};
	Vector2 areaMid2Mid = {screenSize_.x * (2.0f / 3.0f), 1440.0f};
	Vector2 areaRight2Bottom = {screenSize_.x * (3.0f / 3.0f), 2160.0f};
};
