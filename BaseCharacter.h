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

protected: 
	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;
	// テクスチャ
	uint32_t charaTex_ = 0u;
	// 位置
	Vector2 pos_{};
};