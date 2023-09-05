#pragma once
#include "Sprite.h"
#include "Vector2.h"
#include <memory>
#include "Input.h"

/// <summary>
/// ベースキャラクタークラス
/// </summary>
class BaseCharacter {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize();

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

protected: 
	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;
	// テクスチャ
	uint32_t charaTex_ = 0u;
	// 位置
	Vector2 pos_{}; 
	
	// 動くときのフラグ
	bool isMove_ = false;
	// 半径サイズ
	float radius_;

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

	bool IsMove() { return isMove_; }
};