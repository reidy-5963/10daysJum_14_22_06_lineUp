#pragma once
#include "BaseCharacter.h"
#include "ParticleManager.h"

class Enemy : public BaseCharacter {
public: // 継承
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	void AnimationValueInitialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	void SetParticle();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	void OnCollision() override;

	void ParasiteStateUpdate();
	void RootStateUpdate();

	void P2EOnCollision();

	void ParasiteAnimationInitialize();

public: // 設定
	/// <summary>
	/// 死亡フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; }

	bool IsParasite() { return isParasite_; }

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

	/// <summary>
	/// 方向指定
	/// </summary>
	/// <param name="direct"></param>
	void SetRotation(float radian);

	void SetIsDead(bool isDead) { isDead_ = isDead; }
	void SetParticleTex(uint32_t tex) { particleTex_ = tex; }
	void SetParasiteTexture(uint32_t tex) { parasiteTex_ = tex; }
	void SetCollapseTexture(uint32_t tex) { collapseTex_ = tex; }

private:
	Vector2 velocity_ = {};

	bool isDead_ = false;
	bool isPossiblePickUp = false;
	bool isParasite_ = false;
	bool isCollapse_ = false;
	int setTimer = 120;
	int deleteTimer = setTimer;

	uint32_t parasiteTex_ = 0u;
	uint32_t particleTex_ = 0u;
	uint32_t collapseTex_ = 0u;
	std::unique_ptr<ParticleManager> particle_ = nullptr;

private: // 限界値
	Vector2 kMinusLimits = {-250.0f, -250.0f};
	Vector2 kPlusLimits = {1920.0f * 3, 1080.0f * 3};

	int collapseAniTimer = 0;
	int collapseAniNumber = 0;
	int collapseAniScene = 7;
	int collapseAnioneTime = 3;
};
