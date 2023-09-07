#pragma once
#include "Vector2.h"
#include "Enemy.h"
#include<list>

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

	/// <summary>
	/// 弾のリストの取得
	/// </summary>
	/// <returns></returns>
	const std::list<Enemy*>& GetEnemyLists() { return enemys_; }

	void TentRes();

private:
	// 共通テクスチャ
	uint32_t enemyTexture_ = 0u;
	// エネミー
	std::list<Enemy*> enemys_;


	int kEnemyLimit = 8;
	int kRespownTimer = 180;
	int respownCount = kRespownTimer;
};
