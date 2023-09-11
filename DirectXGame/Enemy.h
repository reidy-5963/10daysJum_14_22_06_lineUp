﻿#pragma once
#include "BaseCharacter.h"
#include "ParticleManager.h"

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

	void P2EOnCollision();

	void SetParasiteTexture(uint32_t tex) { parasiteTex_ = tex; }
	void ParasiteAnimationInitialize();

public: // 設定

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

	bool IsParasite() { return isParasite_; }
	
	void SetIsDead(bool isDead) { isDead_ = isDead; }
	void SetParticleTex(uint32_t tex) { particleTex_ = tex; }

private:

	Vector2 velocity_ = {};

	bool isDead_ = false;
	bool isPopUpPlayer = false;
	bool isParasite_ = false;

	int setTimer = 120;
	int deleteTimer = setTimer;

	uint32_t parasiteTex_ = 0u;
	uint32_t particleTex_ = 0u;

	std::unique_ptr<ParticleManager> particle_ = nullptr;

private: // 限界値
	Vector2 kMinusLimits = {-250.0f, -250.0f};
	Vector2 kPlusLimits = {1920.0f * 3, 1080.0f * 3};


};
