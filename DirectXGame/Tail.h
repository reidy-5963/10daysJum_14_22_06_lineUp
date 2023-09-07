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
	/// 弾間隔のカウンター
	/// </summary>
	void BulletTimerCount();

	/// <summary>
	/// 進行方向の更新処理
	/// </summary>
	void DirectionUpdate();

	/// <summary>
	/// 尻尾の動きの更新処理
	/// </summary>
	void MoveUpdate();

	/// <summary>
	/// 弾を打ち出す向きを求める
	/// </summary>
	/// <param name="move">上を-y, 右を+xとして進む値</param>
	/// <returns>弾を打ち出す向き</returns>
	Vector2 BulletDirectionInitialize(Vector2 move);

public: // ゲッター
#pragma region ゲッター
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
#pragma endregion

public: // セッター
#pragma region セッター
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

	void SetBulletRad(float radian) { offsetRadian = radian;
	}
#pragma endregion

private: // メンバ変数
	// 親の位置
	const Vector2* parentPos_;
	// 線形補間するときの終わり位置
	Vector2 lerpEndPos_;

	// 動き出す前にいた位置
	Vector2 prePos_;

	// 尻尾の進行方向
	Vector2 direction_{};
	
	// 何番目の尻尾か
	int tailNo_ = -1;

	// 動くときのための線形補完
	float t_;

	// プレイヤー(頭)がうごいているかフラグ
	bool isPlayerMove_ = false;
	
	// 弾発射フラグ
	bool isFire_ = false;

	// 弾を打ち出す間隔のタイマー
	const int kBulletTime = 60;
	int bulletTimer_ = kBulletTime;

	// プレイヤー
	Player* player_;

	float offsetRadian = 1.0f * 3.14f;
};