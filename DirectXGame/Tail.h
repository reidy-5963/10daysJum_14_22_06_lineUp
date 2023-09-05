#pragma once
#include "BaseCharacter.h"

/// <summary>
/// しっぽ
/// </summary>
class Tail : public BaseCharacter {
public: // メンバ関数
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="parent">親になるスプライトの位置</param>
	/// <param name="tailNo">尻尾番号</param>
	void Initialize(uint32_t texture, const Vector2* parent, int tailNo);

	/// <summary>
	/// 更新処理	
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 尻尾番号の取得
	/// </summary>
	/// <returns>尻尾番号</returns>
	const int GetTailNo() { return tailNo_; }

	/// <summary>
	/// 位置の取得
	/// </summary>
	/// <returns>位置</returns>
	const Vector2 *GetPosition() { return &pos_; }

	/// <summary>
	/// プレイヤーが動いているかのフラグ取得
	/// </summary>
	/// <param name="isMove">プレイヤーが動いているか</param>
	void SetIsMove(bool isMove) { isPlayerMove_ = isMove; }

private: // メンバ変数
	// 
	Vector2 prePos_;
	// 親の位置
	const Vector2* parentPos_;
	Vector2 velo_;
	// 何番目の尻尾か
	int tailNo_ = -1;

	float t_;
	bool isPlayerMove_ = false;
	//bool isMove_ = false;
};