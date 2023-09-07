#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter
{
public:	// 継承
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

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

public:	// 設定

	/// <summary>
	/// 死亡フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// テクスチャの設定
	/// </summary>
	/// <param name="tex"></param>
	void SetTexture(uint32_t tex) { charaTex_ = tex; }

	/// <summary>
	/// 速さの設定
	/// </summary>
	/// <param name="velocity"></param>
	void SetVelocity(Vector2 velocity) { velocity_ = velocity; }

private:

	Vector2 velocity_ = {};

	bool isDead_ = false;

};
