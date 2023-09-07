#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>
#include <cmath>
#include <time.h>
#include <stdlib.h>
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
	//boss_ = std::make_unique<BossEnemy>();
	// 初期化
	//boss_->Initialize();
#pragma endregion

#pragma region 背景
	backTex = TextureManager::Load("testBackTex.png");
	back.reset(Sprite::Create(backTex, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}));
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

	// エネミーの消去処理
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	// エネミーの管理関数
	EnemyManager();

	// プレイヤーの更新処理
	player_->Update();

	// エネミーの更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	// 当たり判定
	CheckAllCollision();

	// ボスの更新処理
	//boss_->Update();

	// 背景の更新処理
	back->SetPosition(backPos - scroll->GetAddScroll());
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

	// エネミーの描画処理
	for (Enemy* enemy : enemys_) {
		enemy->Draw();
	}

	// プレイヤーの描画処理
	player_->Draw();

	// ボスの描画
	//boss_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

/// <summary>
/// 当たり判定
/// </summary>
void GameScene::CheckAllCollision() {
	// リスト取得
	Vector2 targetA, targetB;

	// プレイヤーの弾リストを取得
	const std::list<PlayerBullet*>& playerBullet = player_->GetBullets();

	// プレイヤーと敵の衝突判定
	targetA = player_->GetPosition();

	for (Enemy* enemy : enemys_) {
		// エネミーの位置取得
		targetB = enemy->GetPosition();
		float distance =
		    std::sqrtf(std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));
		float radius = player_->GetRadius() + enemy->GetRadius();
		// 交差判定
		if (distance <= radius) {
			// コールバック
			enemy->OnCollision();

			player_->OnCollision();
		}
	}

	// プレイヤーの弾と敵の衝突判定
	for (PlayerBullet* playerBullet_ : playerBullet) {
		targetA = playerBullet_->GetPosition();
		for (Enemy* enemy : enemys_) {
			targetB = enemy->GetPosition();
			float distance = std::sqrtf(
			    std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));
			float radius = playerBullet_->GetRadius() + enemy->GetRadius();

			if (distance <= radius) {
				// コールバック
				enemy->OnCollision();
				playerBullet_->OnCollision();
			}

		}

	}

}

/// <summary>
/// 敵管理
/// </summary>
void GameScene::EnemyManager() { 
	ImGui::Begin("EnemySetting");
	ImGui::DragInt("Limit", &kEnemyLimit, 1, 0, 20);
	ImGui::DragInt("Respown", &kRespownTimer, 1, 0, 300);
	ImGui::End();

		// 生成（仮
	respownCount--;
	if (respownCount == 0 && enemys_.size() < kEnemyLimit) {
		Enemy* newEnemy = new Enemy();
		newEnemy->Initialize();
		Vector2 res = {
		    float(rand() % 1280 + newEnemy->GetRadius()),
		    float(rand() % 720 + newEnemy->GetRadius())};
		newEnemy->SetPosition(res);
		enemys_.push_back(newEnemy);
		respownCount = kRespownTimer;
	}
}