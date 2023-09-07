#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>
#include <cmath>
#include "Scroll.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	unsigned int currentTime = (int)time(nullptr);
	srand(currentTime);

	// プレイヤー生成
	player_ = std::make_unique<Player>();
	// プレイヤーの初期化処理
	player_->Initialize();

	scroll_ = Scroll::GetInstance(); 
	scroll_->Initialize();
	scroll_->SetTarget(&player_->GetPosition());
	scroll_->SetEdgePos({WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2});
	// ボス生成
	//boss_ = std::make_unique<BossEnemy>();
	// 初期化
	//boss_->Initialize();

	enemyManager_ = EnemyManager::GetInstance();
	enemyManager_->Initialize();

	backTex = TextureManager::Load("testBackTex.png");
	back.reset(Sprite::Create(backTex, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}));
	Vector2 size = back->GetSize();
	back->SetSize({size.x * 1.5f, size.y * 1.5f});


	
}

void GameScene::Update() {
	scroll_->Update();

	enemyManager_->Update();

	// プレイヤーの更新処理
	player_->Update();

	// 当たり判定
	CheckAllCollision();

	// ボスの更新処理
	//boss_->Update();
	Scroll* scroll = Scroll::GetInstance();

	back->SetPosition(backPos - scroll->GetAddScroll());
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
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

	enemyManager_->Draw();

	// プレイヤーの描画処理
	player_->Draw();

	// ボスの描画
	//boss_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() 
{
	// リスト取得
	Vector2 targetA, targetB;

	const std::list<PlayerBullet*>& playerBullet = player_->GetBullets();
	const std::list<Enemy*>& enemys = enemyManager_->GetEnemyLists();

	// プレイヤーと敵の衝突判定
	targetA = player_->GetPosition();
	for (Enemy* enemy : enemys) {
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
		for (Enemy* enemy : enemys) {
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
