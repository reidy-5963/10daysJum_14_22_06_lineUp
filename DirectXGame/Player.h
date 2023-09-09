#pragma once
#include "BaseCharacter.h"
#include "PlayerBullet.h"
#include "Tail.h"
#include <list>


/// <summary>
/// プレイヤー
/// </summary>
class Player : public BaseCharacter {
public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// キー入力での移動処理
	/// </summary>
	void KeyMove();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	void OnCollision() override;
	
	/// <summary>
	/// グローバル変数の初期化処理
	/// </summary>
	void InitializeGrobalVariables() override;

	/// <summary>
	/// グローバル変数の更新処理
	/// </summary>
	void ApplyGrobalVariables() override;

	/// <summary>
	/// カーソルの更新処理
	/// </summary>
	void GetCursor();

	/// <summary>
	/// 弾の更新処理
	/// </summary>
	void BulletUpdate();

	/// <summary>
	/// カーソルの制限処理
	/// </summary>
	void MarkerControl();

	/// <summary>
	/// 左クリックの処理
	/// </summary>
	void LeftClickUpdate();

	/// <summary>
	/// 弾を削除する処理
	/// </summary>
	void DeleteBullet();

	/// <summary>
	/// 通常時の回転処理(パターン2つ)
	/// </summary>
	void RootRotateMove1();
	void RootRotateMove2();
	/// <summary>
	/// 尻尾の更新処理
	/// </summary>
	void TailUpdate();

	void MarkerMovement();

	public: // 追加や削除の関数
	/// <summary>
	/// 尻尾の追加
	/// </summary>
	void AddTails();

	/// <summary>
	/// 尻尾の削除
	/// </summary>
	void DeleteTails();

	/// <summary>
	/// 弾の追加
	/// </summary>
	/// <param name="bullet">追加する弾</param>
	void AddBullets(PlayerBullet* bullet);

	void RootRotateMoveInitialize();

	void RootRotateMoveUpdate();

	void ToMarkerMoveInitialize();

	void ToMarkerMoveUpdate();

public: // Setter & Getter
	/// <summary>
	/// 弾テクスチャの取得
	/// </summary>
	/// <returns>弾テクスチャ</returns>
	uint32_t GetBulletTex() { return bulletTexture_; }

	/// <summary>
	/// マーカーの位置の取得
	/// </summary>
	/// <returns></returns>
	Vector2 &GetMarkerPos() { return markerPos_; }

	/// <summary>
	/// 弾の速さ取得
	/// </summary>
	/// <returns></returns>
	float GetBulletSpeed() { return bulletSpeed_; }

	/// <summary>
	/// 弾のリストの取得
	/// </summary>
	/// <returns></returns>
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }
	
	/// <summary>
	/// 尻尾のリストの取得
	/// </summary>
	/// <returns></returns>
	const std::list<Tail*>& GetTails() { return tails_; }

private: // メンバ変数
	// 入力
	Input* input_ = nullptr;
	
	// マウス位置
	POINT mousePos;

	// プレイヤーの向き
	Vector2 direction_{};

#pragma region マーカー
	// マーカーのスプライト
	std::unique_ptr<Sprite> markerSprite_ = nullptr;

	// マーカーのテクスチャ
	uint32_t markerTex_ = 0u;

	// マーカーの位置
	Vector2 markerPos_{};

	// (前クリックした時のマーカーの位置)
	Vector2 preMarkerPos_{};

	// クリックした位置
	Vector2 clickPos_{};

#pragma endregion

#pragma region 通常時の動き用
	// 回転するための4つ目の点
	Vector2 RotateRootPos_;

	// 回転用の線形補完
	float root_t_ = 0.0f;

	// 回転用のフラグ
	bool isRootMove_ = false;

#pragma endregion
	// 前フレーム時のプレイヤーの位置
	Vector2 clickPlayerPos_{};

	// 前フレーム時の位置
	Vector2 prePos_{};

	// 動くときの線形補完
	float move_t_ = 0.0f;
	float move_t_offset = 0.005f;

	// ベジエで動くときのスタート位置
	Vector2 bezierStartPos_{};

	// ベジエで動くときの終わり位置//
	Vector2 bezierEndPos_{};

	// マーカーを置ける位置の制限値
	// (端から設定した値まではマーカーを置けない)
	float markerLimit_ = 100.0f;

#pragma region 弾
	// 弾
	std::list<PlayerBullet*> bullets_;

	// 弾テクスチャ
	uint32_t bulletTexture_ = 0u;

	// 弾の距離制限
	const float kDeadOffset = 740.0f;

	// 弾の速さ
	float bulletSpeed_ = 20.0f;

#pragma endregion

#pragma region 尻尾
	// 尻尾
	std::list<Tail*> tails_;

	// 尻尾テクスチャ
	uint32_t tailTexture_ = 0u;

	// 尻尾の数
	const int kMaxTail_ = 6;
	float BulletRadian = 0.1f;

#pragma endregion

#pragma region 通常状態の回転用
	//
	Vector2 M2AddRadian[3];
	Vector2 W2AddRadian[3];

	float radianOffset = 3.14f * ( 1.0f / 3.0f);

	bool isMtM1 = false;
	bool isM1tM2 = false;
	bool isM2tP = false;
	bool isPtP1 = false;
	bool isP1tP2 = false;
	bool isP2tM = false;

	float rootRotate_t_offset = 0.04f;
#pragma endregion
	bool ismarkerMove_ = false;
	float markerMove_t = 0.0f;
#ifdef _DEBUG
	// test
	std::unique_ptr<Sprite> m1 = nullptr;
	std::unique_ptr<Sprite> m2 = nullptr;
	std::unique_ptr<Sprite> p1 = nullptr;
	std::unique_ptr<Sprite> p2 = nullptr;
	std::unique_ptr<Sprite> origin_ = nullptr;
	Vector2 originPos_{};
	//

#endif // _DEBUG

	Vector2 shakeVelo_{};
	bool isDamageShake = false;
	int amplitNum = 30;

	int markerAniTimer = 0;
	int markerAniNumber = 0;
	int markerAniScene = 15;
	int markerAniOneTime = 4;
	bool ismarkerAnimation = false;
};
