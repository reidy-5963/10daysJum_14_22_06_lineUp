#pragma once
#include "BaseCharacter.h"

class BossEnemy : public BaseCharacter
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

public:

	void RushAttack();

	void RushAttackSetup();

	void GuidedAttack();

	void GuidedAttackSetup();



};
