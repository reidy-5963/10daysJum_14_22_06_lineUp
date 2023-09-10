#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>
#include <cmath>
#include "Scroll.h"

/// <summary>
/// コンストクラタ
/// </summary>
GameScene::GameScene() {}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene() {}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	unsigned int currentTime = (int)time(nullptr);
	srand(currentTime);

#pragma region プレイヤー
	// プレイヤー生成
	player_ = std::make_unique<Player>();
	// プレイヤーの初期化処理
	player_->Initialize();
#pragma endregion

#pragma region スクロール
	scroll_ = Scroll::GetInstance(); 
	scroll_->Initialize();
	scroll_->SetTarget(&player_->GetPosition());
	scroll_->SetEdgePos({WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2});
#pragma endregion

#pragma region ボス
	// ボス生成
	boss_ = std::make_unique<BossEnemy>();
	// 初期化
	boss_->Initialize();

	enemyManager_ = EnemyManager::GetInstance();
	enemyManager_->Initialize();

	backTex = TextureManager::Load("testBackTex.png");
	back.reset(Sprite::Create(backTex, {0.0f, 0.0f}, {0.2f, 0.2f, 0.2f, 1.0f}, {0.0f, 0.0f}));
	Vector2 size = back->GetSize();
	back->SetSize({size.x * 1.5f, size.y * 1.5f});
#pragma endregion
}

/// <summary>
/// 毎フレーム処理
/// </summary>
void GameScene::Update() { 
	// スクロールの更新処理
	Scroll* scroll = Scroll::GetInstance();
	scroll_->Update();

	// 敵の更新処理
	enemyManager_->SetPlayer(player_->GetPosition());
	enemyManager_->SetSceneVelo(sceneShakevelo_);
	enemyManager_->Update();

	// プレイヤーの更新処理
	player_->Update();

	// 当たり判定
	CheckAllCollision();
	MyMath::ShakeUpdate(sceneShakevelo_, issceneShake, sceneaAmplitNum);
	player_->SetSceneVelo(sceneShakevelo_);
	boss_->SetSceneVelo(sceneShakevelo_);

	if (killCount_ > 10 && !isBossRespown_) {
		//isBossRespown_ = true;
		//boss_->RespownBoss();
	}

	// ボスの更新処理
	//if (!boss_->IsDead()) {
		boss_->SetPlayer(player_->GetPosition());
		boss_->Update();
	//}
	// 背景の更新処理
	back->SetPosition(backPos - scroll->GetAddScroll() + sceneShakevelo_);
}

/// <summary>
/// 描画
/// </summary>
void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	
	// 背景の描画
	back->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// 敵の描画
	enemyManager_->Draw();

	// プレイヤーの描画処理
	player_->Draw();

	// ボスの描画
	if (!boss_->IsDead()) {
		boss_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

/// <summary>
/// 当たり判定
/// </summary>
void GameScene::CheckAllCollision() {
	// 変数の用意
	Vector2 targetA, targetB;
#pragma region
#pragma endregion
	// プレイヤーの弾リストを取得
	const std::list<PlayerBullet*>& playerBullet = player_->GetBullets();
	// 敵
	const std::list<Enemy*>& enemys = enemyManager_->GetEnemyLists();
	// ボスの弾
	const std::list<BossBullet*>& bossBullet = boss_->GetBullets();
	// ファンネル
	const std::list<BossFunnel*>& bossFunnel = boss_->GetFunnels();
	// しっぽ
	const std::list<Tail*>& tails = player_->GetTails();

	// プレイヤーの位置取得
	targetA = player_->GetPosition();

#pragma region 敵とプレイヤー
	for (Enemy* enemy : enemys) {
		// エネミーの位置取得
		targetB = enemy->GetPosition();
		float distance =
		    std::sqrtf(std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));
		float radius = player_->GetRadius() + enemy->GetRadius();
		// 交差判定
		if (distance <= radius) {
			if (!player_->GetIsInvisible()) {
				// コールバック
				if (enemy->IsParasite()) {
					player_->AddTails();
					enemy->SetIsDead(true);
				}
				if (!enemy->IsParasite()) {
					player_->OnCollision();
					enemy->SetIsDead(true);
				}
				//
				killCount_ += 1;

			}
		}
	}
#pragma endregion

#pragma region ボスの弾

	for (BossBullet* bossBullet_ : bossBullet) {
		// エネミーの位置取得
		targetB = bossBullet_->GetPosition();
		float distance =
		    std::sqrtf(std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));
		float radius = player_->GetRadius() + bossBullet_->GetRadius();
		// 交差判定
		if (distance <= radius && !player_->GetIsInvisible()) {
			// コールバック
			bossBullet_->OnCollision();
			player_->OnCollision();
			issceneShake = true;
			sceneaAmplitNum = 40;
		}
	}

#pragma endregion

#pragma region ボスのファンネル

	for (BossFunnel* bossFunnel_ : bossFunnel) {
		// エネミーの位置取得
		targetB = bossFunnel_->GetPosition();
		float distance =
		    std::sqrtf(std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));
		float radius = player_->GetRadius() + bossFunnel_->GetRadius();
		// 交差判定
		if (distance <= radius && !player_->GetIsInvisible()) {
			// コールバック
			bossFunnel_->OnCollision();
			player_->OnCollision();
			issceneShake = true;
			sceneaAmplitNum = 40;

		}
	}

#pragma endregion

#pragma region 敵と尻尾
	for (Enemy* enemy : enemys) {
		// エネミーの位置取得
		targetB = enemy->GetPosition();
		for (Tail* tail : tails) {
			// エネミーの位置取得
			targetA = tail->GetPosition();
			float distance = std::sqrtf(
			    std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));
			float radius = player_->GetRadius() + enemy->GetRadius();
			// 交差判定
			if (distance <= radius) {
				if (!player_->GetIsInvisible()) {
					// コールバック
					enemy->OnCollision();
					tail->OnCollision();
					killCount_ += 1;

				}
			}
		}
	}
#pragma endregion

#pragma region ボスの弾と尻尾
	for (BossFunnel* bossFunnel_ : bossFunnel) {
		// エネミーの位置取得
		targetA = bossFunnel_->GetPosition();
		for (Tail* tail : tails) {
			targetB = tail->GetPosition();
			float distance = std::sqrtf(
			    std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));
			float radius = tail->GetRadius() + bossFunnel_->GetRadius();
			// 交差判定
			if (distance <= radius) {
				// コールバック
				bossFunnel_->OnCollision();
				tail->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region ボスのファンネルと尻尾
	for (BossBullet* bossBullet_ : bossBullet) {
		// エネミーの位置取得
		targetA = bossBullet_->GetPosition();
		for (Tail* tail : tails) {
			targetB = tail->GetPosition();
			float distance = std::sqrtf(
			    std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));
			float radius = tail->GetRadius() + bossBullet_->GetRadius();
			// 交差判定
			if (distance <= radius) {
				// コールバック
				bossBullet_->OnCollision();
				tail->OnCollision();
			}
		}
	}

#pragma endregion

#pragma region プレイヤーの弾と敵
	// プレイヤーの弾と敵の衝突判定
	for (PlayerBullet* playerBullet_ : playerBullet) {
		targetA = playerBullet_->GetPosition();
		for (Enemy* enemy : enemys) {
			targetB = enemy->GetPosition();
			float distance = std::sqrtf(
			    std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));
			float radius = playerBullet_->GetRadius() + enemy->GetRadius();

			if (distance <= radius) {
				// コールバック
				enemy->OnCollision();
				playerBullet_->OnCollision();
				killCount_ += 1;
			}
		}
	}
#pragma endregion

#pragma region プレイヤーの弾とボス
	// 判定
	for (PlayerBullet* playerBullet_ : playerBullet) {
		targetA = playerBullet_->GetPosition();
		targetB = boss_->GetPosition();

		float distance =
		    std::sqrtf(std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));

		float radius = playerBullet_->GetRadius() + boss_->GetRadius();

		if (distance <= radius && boss_->IsAlive()) {
			// コールバック
			playerBullet_->OnCollision();
			boss_->OnCollision();
			boss_->SetHp(boss_->GetHp() - 1);
		}
	}
#pragma endregion
}
