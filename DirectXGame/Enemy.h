#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter
{
public:
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

	/// <summary>
	/// 死亡フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; }

private:

	bool isDead_ = false;

};
