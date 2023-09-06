#pragma once
#include "BaseBullet.h"


/// <summary>
/// 自機の弾
/// </summary>
class PlayerBullet : public BaseBullet {
public: // メンバ関数
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(uint32_t texture, const Vector2& pos, Vector2 velosity) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	void OnCollision() override;

private: // メンバ変数

	const float BulletSpeed = 20.0f;

};