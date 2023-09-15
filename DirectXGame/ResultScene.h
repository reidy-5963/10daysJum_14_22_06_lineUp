#pragma once

#include "BaseScene.h"
#include "Player.h"
#include "Scroll.h"

class ResultScene : public BaseScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	ResultScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResultScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

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

private: // メンバ変数
	// 背景スプライト関係	
	
	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	std::unique_ptr<Sprite> back;
	uint32_t backTex;
	Vector2 backPos = {0.0f, 0.0f};

	std::unique_ptr<Sprite> title_ = nullptr;
	uint32_t titleTex_ = 0u;
	Vector2 radius_{};
	Vector2 titleLogoPos_{};

	Vector2 titleLogoStartPos_{};
	Vector2 titleLogoEndPos_{};
	float titleLogo_t_ = 0.0f;
	bool titleLogoMove_ = false;

	bool isSceneChange = false;
	#pragma region シーン遷移エリア
	std::unique_ptr<Sprite> title2gameSceneAria_ = nullptr;
	Vector2 ariaPos_{};
	uint32_t ariaTex_;
	Vector4 ariaColor_ = {0.6f, 0.6f, 0.6f, 0.6f};

	Vector2 ariaStartPos_{2040.0f, 0.0f};
	Vector2 ariaEndPos_{1800.0f, 0.0f};

	float aria_t_ = 0.0f;
	bool isAriaMove_ = false;
#pragma endregion 
	float bolume = 0.01f;

	uint32_t BGMHandle_ = 0u;
};
