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
	void Tn(int& ten, int& one, int tmp) {
		ten = tmp / 10;
		tmp = tmp % 10;
		one = tmp;
	}

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
	std::unique_ptr<Sprite> back;
	uint32_t backTex;
	Vector2 backPos = {0.0f, 0.0f};
#pragma endregion

#pragma region 音 
	float bolume = 0.01f;

	uint32_t BGMHandle_ = 0u;
#pragma endregion

	int setGameTime = 60 * 60;
	int gameTimer = setGameTime;
	bool isGameSet_ = false;

	std::unique_ptr<Sprite> num[2];
	Vector2 scoreNumPos_{};
	Vector2 numPos[2];
	uint32_t numTex_;

	int scoreTen = 0;
	int scoreOne = 0;

	std::unique_ptr<Sprite> enemyNum[2];
	Vector2 enemyNumPos_{};
	Vector2 enemyNumPos[2];
	int enemyTen = 0;
	int enemyOne = 0;
};