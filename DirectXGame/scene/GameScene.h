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

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene
{

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

	void CheckAllCollision();

	void EnemyManager();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// エネミー
	std::list<Enemy*> enemys_;

	int kEnemyLimit = 8;
	int kRespownTimer = 180;
	int respownCount = kRespownTimer;

	// ボス
	std::unique_ptr<BossEnemy> boss_ = nullptr;

	Scroll* scroll_;

	std::unique_ptr<Sprite> back;
	uint32_t backTex;
	Vector2 backPos = {0.0f, 0.0f};
};
