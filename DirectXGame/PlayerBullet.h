#pragma once
#include "BaseBullet.h"
#include "ParticleManager.h"

/// <summary>
/// 自機の弾
/// </summary>
class PlayerBullet : public BaseBullet {
public: // メンバ関数
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(uint32_t texture, const Vector2& pos, Vector2 velosity) override;

	void SetRadius(float radius) {
		radius_ = radius;
		//sprite_->SetSize({radius * 2, radius * 2});
	}
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

	void SetParticleTex(uint32_t tex) { particleTex_ = tex; }

private: // メンバ変数
	std::unique_ptr<ParticleManager> particle_ = nullptr;
	uint32_t particleTex_ = 0u;
	
};