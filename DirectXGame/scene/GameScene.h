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
	/// 当たり判定
	/// </summary>
	void CheckAllCollision();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Vector2 gameScenePos{};

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// エネミー
	EnemyManager* enemyManager_;

	// ボス
	std::unique_ptr<BossEnemy> boss_ = nullptr;
	
	Scroll* scroll_;

#pragma region 背景
	// 背景スプライト関係
	std::unique_ptr<Sprite> back;
	uint32_t backTex;
	Vector2 backPos = {0.0f, 0.0f};
#pragma endregion
};
