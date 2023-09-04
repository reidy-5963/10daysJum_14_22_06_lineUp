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

private: // メンバ変数	
	Input* input_ = nullptr;
	POINT mousePos;
	
	Vector2 markerPos_{};
};