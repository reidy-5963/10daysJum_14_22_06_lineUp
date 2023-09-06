#pragma once
#include "BaseCharacter.h"
#include <list>

class Player;

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
	/// 弾の発射処理
	/// </summary>
	void Fire();

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
	/// 攻撃フラグの取得
	/// </summary>
	/// <returns>攻撃フラグ</returns>
	bool IsFire() { return isFire_; }

	/// <summary>
	///  尻尾の進行方向の取得
	/// </summary>
	/// <returns>尻尾の進行方向</returns>
	Vector2 GetDirection() { return direction_; }

 
	/// <summary>
	/// プレイヤーが動いているかのフラグ設定
	/// </summary>
	/// <param name="isMove">プレイヤーが動いているか</param>
	void SetIsMove(bool isMove) { isPlayerMove_ = isMove; }

	/// <summary>
	/// 攻撃フラグの設定
	/// </summary>
	/// <param name="isFire">攻撃フラグ</param>
	void SetIsFire(bool isFire) { isFire_ = isFire; }

	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) { player_ = player; }

private: // メンバ変数
	// 
	Vector2 prePos_;
	// 親の位置
	const Vector2* parentPos_;
	Vector2 lerpEndPos_;

	Vector2 direction_{};
	// 何番目の尻尾か
	int tailNo_ = -1;

	float t_;
	bool isPlayerMove_ = false;
	
	// 弾発射フラグ
	bool isFire_ = false;

	const int kBulletTime = 60;
	int bulletTimer_ = kBulletTime;

	Player* player_;
};