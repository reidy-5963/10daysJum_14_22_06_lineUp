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

public:	// リスポーンタイプ
	/// <summary>
	/// 適当湧き
	/// </summary>
	void TentRes();

	void CreateEnemy(int spownPoint);

	Vector2 RandomRadianVector();

private:

	Input* input_ = nullptr;

	// 共通テクスチャ
	uint32_t charaTex_ = 0u;
	// エネミー
	std::list<Enemy*> enemys_;

	Vector2 playerPos_ = {};

	int kEnemyLimit = 8;
	int kRespownTimer = 180;
	int respownCount = kRespownTimer;

	int ArrowCoolTime = 0;

	bool isRespown = false;

	int resPattern;

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
	int kRespownTimer_ = 10 * 60;
	int respownTimer_ = 0;

	Vector2 resCircle_ = {};
	float resCircleRadius_ = 0;

	enum SpownPoint {
		kLeftTop,
		kLeftBottom,
		kRightTop,
		kRightBottom,
	};

};
