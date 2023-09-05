#pragma once
#include "Sprite.h"
#include "Vector2.h"
#include <memory>


/// <summary>
/// 弾のベースクラス
/// </summary>
class BaseBullet {
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize(uint32_t texture, const Vector2& pos, Vector2 velosity);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

protected: // メンバ変数
	// スプライト
	std::unique_ptr<Sprite> sprite_;
	uint32_t texture_;

	// 位置
	Vector2 pos_;

	// 速度
	Vector2 velocity_;




};
