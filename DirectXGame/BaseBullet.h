#pragma once
#include "Sprite.h"
#include "Vector2.h"
#include <memory>


/// <summary>
/// 弾のベースクラス
/// </summary>
class BaseBullet {
public: // メンバ変数
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

	/// <summary>
	/// 衝突処理
	/// </summary>
	virtual void OnCollision();

	void SetSize(Vector2 size);

	Vector2 GetSize();

protected: // メンバ変数
	// スプライト
	std::unique_ptr<Sprite> sprite_;
	uint32_t texture_;

	// 位置
	Vector2 pos_;

	// 速度
	Vector2 velocity_;

	bool isDead_;

	float radius_;

	float bulletSpeed_;

public:
	/// <summary>
	/// 座標のSetter
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(Vector2& pos) { pos_ = pos; }

	/// <summary>
	/// 座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector2 GetPosition() { return pos_; }

	/// <summary>
	/// 半径のSetter
	/// </summary>
	/// <param name="rad"></param>
	void SetRadius(float rad) { radius_ = rad; }

	/// <summary>
	/// 半径のGetter
	/// </summary>
	/// <returns></returns>
	float GetRadius() { return radius_; }

	/// <summary>
	/// フラグの取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// フラグの設定
	/// </summary>
	/// <param name="dead"></param>
	void SetIsDead(bool dead) { isDead_ = dead; }

};
