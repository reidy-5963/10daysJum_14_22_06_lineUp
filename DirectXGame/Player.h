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

#pragma region 初期化系
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 尻尾に関する初期化処理
	/// </summary>
	void TailInitialize();

	/// <summary>
	/// 弾に関する初期化処理
	/// </summary>
	void BulletInitialize();

	/// <summary>
	///  アニメーションに使う変数の初期化処理
	/// </summary>
	void AnimationValueInitialize();

	/// <summary>
	/// UIに関する初期化処理
	/// </summary>
	void UIInitialize();
	
	/// <summary>
	/// マーカーに関する初期化処理
	/// </summary>
	void MarkerInitialize(); 
	
	/// <summary>
	/// グローバル変数の初期化処理
	/// </summary>
	void InitializeGrobalVariables() override;
	
	/// <summary>
	/// 通常状態の回転の初期化処理
	/// </summary>
	void RootRotateMoveInitialize();

	/// <summary>
	/// マーカーへ向かう動きの初期化処理
	/// </summary>
	void ToMarkerMoveInitialize();

#pragma endregion

#pragma region 更新系
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// グローバル変数の更新処理
	/// </summary>
	void ApplyGrobalVariables() override;

	/// <summary>
	/// キー入力での移動処理
	/// </summary>
	void KeyMove();

	/// <summary>
	/// 左クリックの処理
	/// </summary>
	void LeftClickUpdate();

	/// <summary>
	/// カーソルの更新処理
	/// </summary>
	void GetCursor();

	/// <summary>
	/// カーソルの制限処理
	/// </summary>
	void MarkerControl();

	/// <summary>
	/// マーカーの動きの処理
	/// </summary>
	void MarkerMovement();

	/// <summary>
	/// 弾の更新処理
	/// </summary>
	void BulletUpdate();

	/// <summary>
	/// 弾を削除する処理
	/// </summary>
	void DeleteBulletUpdate();

	/// <summary>
	/// 尻尾の更新処理
	/// </summary>
	void TailUpdate();

	/// <summary>
	/// 通常状態の回転処理
	/// </summary>
	void RootRotateMoveUpdate();

	/// <summary>
	/// 通常時の回転パターン(パターン2つ)
	/// </summary>
	void RootRotateMove1();
	void RootRotateMove2();

	/// <summary>
	/// マーカーへ向かう動きの処理
	/// </summary>
	void ToMarkerMoveUpdate();

#pragma endregion

#pragma region 描画系
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// UI系の描画処理
	/// </summary>
	void DrawUI();

#pragma endregion
	
public: // 追加や削除の関数
#pragma region 追加や削除の関数
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
#pragma endregion

public: // Setter & Getter
#pragma region setter_getter
	/// <summary>
	/// 弾テクスチャの取得
	/// </summary>
	/// <returns>弾テクスチャ</returns>
	uint32_t GetBulletTex() { return bulletTexture_; }

	/// <summary>
	/// マーカーの位置の取得
	/// </summary>
	/// <returns>マーカーの位置</returns>
	Vector2 &GetMarkerPos() { return markerPos_; }

	/// <summary>
	/// 弾の速さ取得
	/// </summary>
	/// <returns>弾の速さ</returns>
	float GetBulletSpeed() { return bulletSpeed_; }

	/// <summary>
	/// 無敵中フラグ取得
	/// </summary>
	/// <returns>無敵中フラグ</returns>
	int GetIsInvisible() { return isInvisible_; }

	/// <summary>
	/// 弾のリストの取得
	/// </summary>
	/// <returns>弾のリスト</returns>
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }
	
	/// <summary>
	/// 尻尾のリストの取得
	/// </summary>
	/// <returns>尻尾のリスト</returns>
	const std::list<Tail*>& GetTails() { return tails_; }

	/// <summary>
	/// 尻尾の数の取得
	/// </summary>
	/// <returns>尻尾の数</returns>
	const int GetTail() { return int(tails_.size()); }

	bool IsDead() { return isDead_; }

	void SetIsGameStart(bool isStart) { isGameStart = isStart; }
#pragma endregion

private: // メンバ変数
	// 入力
	Input* input_ = nullptr;
	
	// マウス位置
	POINT mousePos;

	// 前フレーム時のプレイヤーの位置
	Vector2 clickPlayerPos_{};

	// 前フレーム時の位置
	Vector2 prePos_{};

	// プレイヤーの向き
	Vector2 direction_{};

	// プレイヤーの残機(尻尾の数)
	const int setDamageCount = 3;
	int damageCount = setDamageCount;
	int scoreCOunt = 0;
#pragma region 当たり判定処理系

	int kInvisibleTimer_ = 90;

	int invisibleTimeCount_;

	bool isInvisible_ = false;

#pragma endregion

#pragma region マーカー
	// マーカーのスプライト
	std::unique_ptr<Sprite> markerSprite_ = nullptr;
	float markerRadius_ = 32.0f;
	// マーカーのテクスチャ
	uint32_t markerTex_ = 0u;

	// マーカーの位置
	Vector2 markerPos_{};

	// (前クリックした時のマーカーの位置)
	Vector2 preMarkerPos_{};

	// クリックした位置
	Vector2 clickPos_{};

	// マーカーの線形補完中フラグ
	bool ismarkerMove_ = false;
	// マーカーの線形補完用t
	float markerMove_t = 0.0f;

	// マーカーを置ける位置の制限値
	// (端から設定した値まではマーカーを置けない)
	float markerLimit_ = 100.0f;

#pragma region マーカーのアニメーション用 
	int markerAniTimer = 0;
	int markerAniNumber = 0;
	int markerAniScene = 15;
	int markerAniOneTime = 4;
	bool ismarkerAnimation = false;
#pragma endregion

	uint32_t yosokusenTex_;
	std::unique_ptr<Sprite> yosokusen_;
	Vector2 yosokusenSize{};
	Vector2 yosokusenPos_{};
#pragma endregion

#pragma region 通常時の動き用
	// 回転するための4つ目の点
	Vector2 RotateRootPos_;

	// 回転用の線形補完
	float root_t_ = 0.0f;

	// 回転用のフラグ
	bool isRootMove_ = false;

#pragma endregion

#pragma region マーカーへ向かう処理用
	// 動くときの線形補完
	float move_t_ = 0.0f;
	float move_t_offset = 0.005f;

	// ベジエで動くときのスタート位置
	Vector2 bezierStartPos_{};

	// ベジエで動くときの終わり位置//
	Vector2 bezierEndPos_{};
#pragma endregion

#pragma region 弾
	// 弾
	std::list<PlayerBullet*> bullets_;

	// 弾テクスチャ
	uint32_t bulletTexture_ = 0u;

	// 弾の距離制限
	const float kDeadOffset = 1200.0f;

	// 弾の速さ
	float bulletSpeed_ = 20.0f;
	uint32_t bulletParticle_ = 0u;
#pragma endregion

#pragma region 尻尾
	// 尻尾
	std::list<Tail*> tails_;

	// 尻尾テクスチャ
	uint32_t tailTexture_[4];

	// 尻尾の数
	const int kMaxTail_ = 6;
	float BulletRadian = 0.1f;

#pragma endregion

#pragma region 通常状態の回転用
	//
	Vector2 M2AddRadian[3];
	Vector2 W2AddRadian[3];
	// 通常回転時の中心点
	Vector2 originPos_{};

	float radianOffset = 3.14f * ( 1.0f / 3.0f);

	bool isMtM1 = false;
	bool isM1tM2 = false;
	bool isM2tP = false;
	bool isPtP1 = false;
	bool isP1tP2 = false;
	bool isP2tM = false;

	float rootRotate_t_offset = 0.04f;
#pragma endregion
	
#pragma region プレイヤーの揺れ用
	Vector2 shakeVelo_{};
	bool isDamageShake = false;
	int amplitNum = 30;
#pragma endregion

#pragma region UI
	// UIのスプライト
	std::unique_ptr<Sprite> playerUI_;

	// UIの位置
	Vector2 UIPlayerPos_{100.0f, 100.0f};
#pragma endregion

#ifdef _DEBUG
	// test
	std::unique_ptr<Sprite> m1 = nullptr;
	std::unique_ptr<Sprite> m2 = nullptr;
	std::unique_ptr<Sprite> p1 = nullptr;
	std::unique_ptr<Sprite> p2 = nullptr;
	std::unique_ptr<Sprite> origin_ = nullptr;
	//
#endif // _DEBUG
	bool isDead_ = false;

	bool isGameStart = false;

	const int setRefreshTime = 40;
	int refreshTimer_ = setRefreshTime;
};
