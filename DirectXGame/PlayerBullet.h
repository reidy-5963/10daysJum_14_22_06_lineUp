#pragma once
#include "Sprite.h"
#include "Vector2.h"
#include <memory>

/// <summary>
/// 自機の弾
/// </summary>
class PlayerBullet {
public: // メンバ関数
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(uint32_t texture);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private: // メンバ変数
	// スプライト
	std::unique_ptr<Sprite> sprite_;
	uint32_t texture_;

	// 位置
	Vector2 pos_;

	// 速度
	Vector2 velocity_;


};