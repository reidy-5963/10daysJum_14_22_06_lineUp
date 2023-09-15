﻿#pragma once

#include "BaseScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Scroll.h"
#include "EnemyManager.h"

class TitleScene : public BaseScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	void StaticValueInitialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	void NotTutorialUpdate();
	void TutorialUpdate();

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
	void CheckAllCollision();

private: // メンバ変数
#pragma region 背景
	// 背景スプライト関係
	std::unique_ptr<Sprite> back;
	uint32_t backTex;
	Vector2 backPos = {0.0f, 0.0f};

#pragma endregion

#pragma region シーン遷移エリア 
	std::unique_ptr <Sprite> title2gameSceneAria_ = nullptr;
	Vector2 ariaPos_{};
	uint32_t ariaTex_;
	Vector4 ariaColor_ = {0.6f, 0.6f, 0.6f, 0.6f};

	Vector2 ariaStartPos_{2040.0f, 0.0f};
	Vector2 ariaEndPos_{1800.0f, 0.0f};

	float aria_t_ = 0.0f;
	bool isAriaMove_ = false;
#pragma endregion

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	std::unique_ptr<Sprite> titleLogo_ = nullptr;
	uint32_t titleLogo_tex_ = 0u;
	Vector2 titleLogo_pos_{};

	Vector2 titleLogoStartPos_{};
	Vector2 titleLogoEndPos_{};
	float titleLogo_t_ = 0.0f;
	bool titleLogoMove_ = false;

	bool TitleTutrialStart = false;
	// エネミー
	EnemyManager* enemyManager_;
	const int enemyNum = 4;
	Vector2 enemyEndPos_[6];
	float bolume = 0.01f;
	float SEvolume = 0.1f;

	uint32_t BGMHandle_ = 0u;

	std::unique_ptr<Sprite> leftClick_ = nullptr;
	uint32_t leftClick_tex_ = 0u;
	Vector2 leftClick_pos_{};

	uint32_t pickUpTailSEHandle_;

	Vector2 leftClick_StartPos_{};
	Vector2 leftClick_EndPos_{};
	float leftClick_t_ = 0.0f;
	bool leftClick_Move_ = false;

	uint32_t hitTex_;
	std::unique_ptr<Sprite> HitText_[4];
	Vector2 HitPos_[4];
	uint32_t pickedTex_;
	std::unique_ptr<Sprite> PickedText_[4];
	Vector2 PickedPos_[4];
};
