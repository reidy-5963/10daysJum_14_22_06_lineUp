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

	const int GetTailNo() { return tailNo_; }

	const Vector2 *GetPosition() { return &pos_; }

	void SetIsMove(bool isMove) { isPlayerMove_ = isMove; }

	void SetVelosity(Vector2 velo) { velo_ = velo;
	}

	//const float* GetT() { return t_; }
	//void SetT(const float* t) { t_ = t; }

private: // メンバ変数
	const int setPrePosTime = 10;
	int prePosTimer = setPrePosTime;

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