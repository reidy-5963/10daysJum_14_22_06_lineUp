#pragma once
#include "Sprite.h"
#include "MyMath.h"
#include <memory>
#include "Input.h"

/// <summary>
/// ベースキャラクタークラス
/// </summary>
class BaseCharacter {
public: // メンバ関数
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

	virtual void InitializeGrobalVariables();

	virtual void ApplyGrobalVariables();
	void ScreenPosInitialize() { ScreenPos = {}; }
	void AddScreenPos(Vector2 pos) { ScreenPos += pos;
	}

protected: // 継承限定メンバ変数
	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;
	// テクスチャ
	uint32_t charaTex_ = 0u;
	// 位置
	Vector2 pos_{};
	Vector2 ScreenPos{};
	Vector4 color_{1.0f, 1.0f, 1.0f, 1.0f};
	Vector2 sceneVelo{};

	// 動くときのフラグ
	bool isMove_ = false;
	// 半径サイズ
	float radius_;

	int animationTimer = 0;
	int animationNumber = 0;
	int animationScene = 5;
	int oneTime = 20;
	bool isAnimation = false;

public: // セッター関数
	/// <summary>
	/// 座標のSetter
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(Vector2& pos) { pos_ = pos; }

	/// <summary>
	/// 座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector2 &GetPosition() { return pos_; }

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
	/// 動いているかのフラグを取得
	/// </summary>
	/// <returns></returns>
	bool IsMove() { return isMove_; }
	void SetSceneVelo(Vector2 velo) { sceneVelo = velo; }
};