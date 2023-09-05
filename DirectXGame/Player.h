#pragma once
#include "BaseCharacter.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player : public BaseCharacter {
public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	void KeyMove();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	void OnCollision() override;

private: // メンバ変数	
	Input* input_ = nullptr;
	POINT mousePos;
	
	Vector2 markerPos_{};
	Vector2 preMarkerPos_{};

	Vector2 prePos_{};	

	bool isMove_ = false;
	float move_t_ = 0.0f;

	Vector2 bezierStartPos_{};
	Vector2 bezierEndPos_{};

	uint32_t markerTex_ = 0u;
	std::unique_ptr<Sprite> markerSprite_ = nullptr;
};