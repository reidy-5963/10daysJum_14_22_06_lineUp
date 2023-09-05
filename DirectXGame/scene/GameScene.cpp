#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();


	// プレイヤー生成
	player_ = std::make_unique<Player>();
	// プレイヤーの初期化処理
	player_->Initialize();

	// エネミー生成
	enemy_ = std::make_unique<Enemy>();
	// エネミーの初期化処理
	enemy_->Initialize();

}

void GameScene::Update() {
	// プレイヤーの更新処理
	player_->Update();
	respownCount--;
	if (respownCount == 0) {
		cou++;
		Vector2 res = {cou * 50.0f, 100.0f};
		Enemy* newEnemy = new Enemy();
		newEnemy->Initialize();
		newEnemy->SetPosition(res);
		enemys_.push_back(newEnemy);
		respownCount = kRespownTimer;
	}

	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	// エネミー
	//enemy_->Update();

	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	CheckAllCollision();

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
	//enemy_->Draw();
	for (Enemy* enemy : enemys_) {
		enemy->Draw();
	}

	// プレイヤーの描画処理
	player_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() 
{
	// リスト取得
	Vector2 targetA, targetB;
	targetA = player_->GetPosition();
	for (Enemy* enemy : enemys_) {
		targetB = enemy->GetPosition();

		float distance = powf(targetA.x - targetB.x, 2) + powf(targetA.y - targetB.y, 2);
		float radius = player_->GetRadius() + enemy->GetRadius();
		// 交差判定
		if (distance <= radius) {
			// コールバック
			enemy->OnCollision();

			player_->OnCollision();
		}

	}
}
