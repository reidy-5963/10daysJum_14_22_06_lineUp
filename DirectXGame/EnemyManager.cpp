#include "EnemyManager.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <time.h>
#include <stdlib.h>
#include "WinApp.h"
#include <cmath>
#include <numbers>
#include "MyMath.h"

EnemyManager* EnemyManager::GetInstance() 
{
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize() { 
	input_ = Input::GetInstance();

	charaTex_ = TextureManager::Load("Enemy_ver2.png");
}

void EnemyManager::Update() 
{
	ImGui::Begin("count");
	ImGui::Text("isRespown : %d", isRespown);
	ImGui::Text("arrowCoolTime : %d", ArrowCoolTime);
	ImGui::End();

	respownTimer_++;

	if (respownTimer_ % kRespownTimer_ == 0) {
		CreateEnemy(kLeftTop);
		CreateEnemy(kLeftBottom);
		CreateEnemy(kRightTop);
		CreateEnemy(kRightBottom);
	}

	if (input_->TriggerKey(DIK_4)) {
		CreateEnemy(kLeftTop);
		CreateEnemy(kLeftBottom);
		CreateEnemy(kRightTop);
		CreateEnemy(kRightBottom);
	}

	if (input_->TriggerKey(DIK_5)) {
		this->DiagonalBehavior();
	}

	if (input_->TriggerKey(DIK_6)) {
		this->DiagonalClockWiseBehavior();
	}

	ArrowBehaviorControl();

	// 消去処理
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	//TentRes();

	// エネミーの更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
}

void EnemyManager::Draw() 
{
	// エネミーの描画処理
	for (Enemy* enemy : enemys_) {
		enemy->Draw();
	}
}

void EnemyManager::TentRes() 
{
	ImGui::Begin("EnemySetting");
	ImGui::DragInt("Limit", &kEnemyLimit, 1, 0, 20);
	ImGui::DragInt("Respown", &kRespownTimer, 1, 0, 300);
	ImGui::End();

	// 生成（仮
	respownCount--;
	if (respownCount == 0 && enemys_.size() < kEnemyLimit) {
		Enemy* newEnemy = new Enemy();
		newEnemy->SetTexture(this->charaTex_);
		newEnemy->Initialize();
		Vector2 res = {
		    float(rand() % 1280 + newEnemy->GetRadius()),
		    float(rand() % 720 + newEnemy->GetRadius())};
		newEnemy->SetPosition(res);
		enemys_.push_back(newEnemy);
		respownCount = kRespownTimer;
	}
}

void EnemyManager::CreateEnemy(int spownPoint) 
{
	Enemy* newEnemy = new Enemy();
	newEnemy->SetTexture(charaTex_);
	newEnemy->Initialize();
	Vector2 spownPos = {};

	switch (spownPoint) {
	case kLeftTop:
		spownPos = {
		    playerPos_.x - float(WinApp::kWindowWidth / 2),
		    playerPos_.y - float(WinApp::kWindowHeight / 2) + 10.0f};
		break;
	case kLeftBottom:
		spownPos = {
		    playerPos_.x - float(WinApp::kWindowWidth / 2),
		    playerPos_.y + float(WinApp::kWindowHeight / 2)};
		break;
	case kRightTop:
		spownPos = {
		    playerPos_.x + float(WinApp::kWindowWidth / 2),
		    playerPos_.y - float(WinApp::kWindowHeight / 2) + 10.0f};
		break;
	case kRightBottom:
		spownPos = {
		    playerPos_.x + float(WinApp::kWindowWidth / 2),
		    playerPos_.y + float(WinApp::kWindowHeight / 2)};
		break;
	}

	newEnemy->SetPosition(spownPos);
	//newEnemy->SetVelocity(Vector2(5.0f, 0));
	//newEnemy->SetVelocity(MyMath::Normalize(playerPos_ - newEnemy->GetPosition()));
	newEnemy->SetVelocity(RandomRadianVector());
	enemys_.push_back(newEnemy);
	
}

Vector2 EnemyManager::RandomRadianVector() 
{
	int random = int(rand()) % 20 + 1;
	int degree = 360 / random;
	float radian = (float(degree) * (float)std::numbers::pi) / 180;
	Vector2 result = {1 * std::cosf(radian), 1 * std::sinf(radian)};
	return result; 
}

void EnemyManager::DiagonalBehavior() 
{ 
	float enemySpaceOffset = 50.0f;

	int DiagonalNumber = 3;

	/// 左上
	Vector2 offset = {
	playerPos_.x - float(WinApp::kWindowWidth) / 2 - 200.0f,
	playerPos_.y - float(WinApp::kWindowHeight) / 2 - 150.0f};

	for (int i = 0; i < DiagonalNumber; i++) {
		// 生成・初期化
		Enemy* newEnemy = new Enemy();
		newEnemy->SetTexture(charaTex_);
		newEnemy->Initialize();

		// 座標設定
		Vector2 newPos = {offset.x + (enemySpaceOffset * i), offset.y - (enemySpaceOffset * i)};
		newEnemy->SetPosition(newPos);

		// 角度取得
		// 度数
		float degree = 315.0f;
		// ラジアン
		float radian = degree * ((float)std::numbers::pi / 180.0f);
		// 速さ
		float moveSpeed = 5.0f;
		// 角度に合わせて正規化
		Vector2 norm = {std::cosf(radian), -std::sinf(radian)};
		norm = MyMath::Normalize(norm);
		// 速さ設定
		newEnemy->SetVelocity(
		    Vector2(norm.x * moveSpeed, norm.y * moveSpeed));
		// リストに追加
		enemys_.push_back(newEnemy);
	}

	/// 左下
	offset = {
	    playerPos_.x - float(WinApp::kWindowWidth) / 2 - 200.0f,
	    playerPos_.y + float(WinApp::kWindowHeight) / 2 + 150.0f};

	for (int i = 0; i < DiagonalNumber; i++) {
		// 生成・初期化
		Enemy* newEnemy = new Enemy();
		newEnemy->SetTexture(charaTex_);
		newEnemy->Initialize();

		// 座標設定
		Vector2 newPos = {offset.x + (enemySpaceOffset * i), offset.y + (enemySpaceOffset * i)};
		newEnemy->SetPosition(newPos);

		// 角度取得
		// 度数
		float degree = 45.0f;
		// ラジアン
		float radian = degree * ((float)std::numbers::pi / 180.0f);
		// 速さ
		float moveSpeed = 5.0f;
		// 角度に合わせて正規化
		Vector2 norm = {std::cosf(radian), -std::sinf(radian)};
		norm = MyMath::Normalize(norm);
		// 速さ設定
		newEnemy->SetVelocity(Vector2(norm.x * moveSpeed, norm.y * moveSpeed));
		// リストに追加
		enemys_.push_back(newEnemy);
	}

	/// 右上
	offset = {
	playerPos_.x + float(WinApp::kWindowWidth) / 2 + 200.0f,
	playerPos_.y - float(WinApp::kWindowHeight) / 2 - 150.0f};

	for (int i = 0; i < DiagonalNumber; i++) {
		// 生成・初期化
		Enemy* newEnemy = new Enemy();
		newEnemy->SetTexture(charaTex_);
		newEnemy->Initialize();

		// 座標設定
		Vector2 newPos = {offset.x - (enemySpaceOffset * i), offset.y - (enemySpaceOffset * i)};
		newEnemy->SetPosition(newPos);

		// 角度取得
		// 度数
		float degree = 180.0f + 45.0f;
		// ラジアン
		float radian = degree * ((float)std::numbers::pi / 180.0f);
		// 速さ
		float moveSpeed = 5.0f;
		// 角度に合わせて正規化
		Vector2 norm = {std::cosf(radian), -std::sinf(radian)};
		norm = MyMath::Normalize(norm);
		// 速さ設定
		newEnemy->SetVelocity(Vector2(norm.x * moveSpeed, norm.y * moveSpeed));
		// リストに追加
		enemys_.push_back(newEnemy);
	}

	/// 右下
	offset = {
	    playerPos_.x + float(WinApp::kWindowWidth) / 2 + 200.0f,
	    playerPos_.y + float(WinApp::kWindowHeight) / 2 + 150.0f};

	for (int i = 0; i < DiagonalNumber; i++) {
		// 生成・初期化
		Enemy* newEnemy = new Enemy();
		newEnemy->SetTexture(charaTex_);
		newEnemy->Initialize();

		// 座標設定
		Vector2 newPos = {offset.x - (enemySpaceOffset * i), offset.y + (enemySpaceOffset * i)};
		newEnemy->SetPosition(newPos);

		// 角度取得
		// 度数
		float degree = 135.0f;
		// ラジアン
		float radian = degree * ((float)std::numbers::pi / 180.0f);
		// 速さ
		float moveSpeed = 5.0f;
		// 角度に合わせて正規化
		Vector2 norm = {std::cosf(radian), -std::sinf(radian)};
		norm = MyMath::Normalize(norm);
		// 速さ設定
		newEnemy->SetVelocity(Vector2(norm.x * moveSpeed, norm.y * moveSpeed));
		// リストに追加
		enemys_.push_back(newEnemy);
	}
}

void EnemyManager::DiagonalClockWiseBehavior() 
{
	float enemySpaceOffset = 50.0f;

	int DiagonalNumber = 3;

	/// 左上
	Vector2 offset = {
	    playerPos_.x - float(WinApp::kWindowWidth) / 2 + 200.0f,
	    playerPos_.y - float(WinApp::kWindowHeight) / 2 + 150.0f};

	for (int i = 0; i < DiagonalNumber; i++) {
		// 生成・初期化
		Enemy* newEnemy = new Enemy();
		newEnemy->SetTexture(charaTex_);
		newEnemy->Initialize();

		// 座標設定
		Vector2 newPos = {offset.x + (enemySpaceOffset * i), offset.y - (enemySpaceOffset * i)};
		newEnemy->SetPosition(newPos);

		// 速さ
		float moveSpeed = 5.0f;
		// 速さ設定
		newEnemy->SetVelocity(Vector2(moveSpeed, 0));
		// リストに追加
		enemys_.push_back(newEnemy);
	}

	/// 左下
	offset = {
	    playerPos_.x - float(WinApp::kWindowWidth) / 2 + 200.0f,
	    playerPos_.y + float(WinApp::kWindowHeight) / 2 - 150.0f};

	for (int i = 0; i < DiagonalNumber; i++) {
		// 生成・初期化
		Enemy* newEnemy = new Enemy();
		newEnemy->SetTexture(charaTex_);
		newEnemy->Initialize();

		// 座標設定
		Vector2 newPos = {offset.x + (enemySpaceOffset * i), offset.y + (enemySpaceOffset * i)};
		newEnemy->SetPosition(newPos);

		// 速さ
		float moveSpeed = 5.0f;
		// 速さ設定
		newEnemy->SetVelocity(Vector2(0, -moveSpeed));
		// リストに追加
		enemys_.push_back(newEnemy);
	}

	/// 右上
	offset = {
	    playerPos_.x + float(WinApp::kWindowWidth) / 2 - 200.0f,
	    playerPos_.y - float(WinApp::kWindowHeight) / 2 + 150.0f};

	for (int i = 0; i < DiagonalNumber; i++) {
		// 生成・初期化
		Enemy* newEnemy = new Enemy();
		newEnemy->SetTexture(charaTex_);
		newEnemy->Initialize();

		// 座標設定
		Vector2 newPos = {offset.x - (enemySpaceOffset * i), offset.y - (enemySpaceOffset * i)};
		newEnemy->SetPosition(newPos);

		// 速さ
		float moveSpeed = 5.0f;
		// 速さ設定
		newEnemy->SetVelocity(Vector2(0, moveSpeed));
		// リストに追加
		enemys_.push_back(newEnemy);
	}

	/// 右下
	offset = {
	    playerPos_.x + float(WinApp::kWindowWidth) / 2 - 200.0f,
	    playerPos_.y + float(WinApp::kWindowHeight) / 2 - 150.0f};

	for (int i = 0; i < DiagonalNumber; i++) {
		// 生成・初期化
		Enemy* newEnemy = new Enemy();
		newEnemy->SetTexture(charaTex_);
		newEnemy->Initialize();

		// 座標設定
		Vector2 newPos = {offset.x - (enemySpaceOffset * i), offset.y + (enemySpaceOffset * i)};
		newEnemy->SetPosition(newPos);

		// 速さ
		float moveSpeed = 5.0f;
		// 速さ設定
		newEnemy->SetVelocity(Vector2(-moveSpeed, 0));
		// リストに追加
		enemys_.push_back(newEnemy);
	}
}

void EnemyManager::ArrowBehavior(int switchPatt) 
{
	// 敵同士の間隔
	float enemySpaceOffset = 50.0f;
	// 数
	int ArrowNumber = 5;
	// プレイヤーからの距離
	Vector2 offset = {};
	// 速さ
	float moveSpeed = 5.0f;

	switch (switchPatt) {
	case kDown:
		// 上から下
		offset = {plPrevPos_.x, plPrevPos_.y - float(WinApp::kWindowHeight / 2)};
		for (int i = 0; i < ArrowNumber; i++) {
			// 生成・初期化
			Enemy* newEnemy = new Enemy();
			newEnemy->SetTexture(charaTex_);
			newEnemy->Initialize();

			/// 左側
			// 座標設定
			Vector2 newPos = {offset.x - (enemySpaceOffset * i), offset.y - (enemySpaceOffset * i)};
			newEnemy->SetPosition(newPos);
			// 速さ設定
			newEnemy->SetVelocity(Vector2(0, moveSpeed));
			// リストに追加
			enemys_.push_back(newEnemy);
		}
		for (int i = 1; i < ArrowNumber; i++) {
			// 生成・初期化
			Enemy* newEnemy = new Enemy();
			newEnemy->SetTexture(charaTex_);
			newEnemy->Initialize();

			/// 右側
			// 座標設定
			Vector2 newPos = {offset.x + (enemySpaceOffset * i), offset.y - (enemySpaceOffset * i)};
			newEnemy->SetPosition(newPos);
			// 速さ設定
			newEnemy->SetVelocity(Vector2(0, moveSpeed));
			// リストに追加
			enemys_.push_back(newEnemy);
		}
		break;

	case kUp:
		// 下から上
		offset = {plPrevPos_.x, plPrevPos_.y + float(WinApp::kWindowHeight / 2)};
		for (int i = 0; i < ArrowNumber; i++) {
			// 生成・初期化
			Enemy* newEnemy = new Enemy();
			newEnemy->SetTexture(charaTex_);
			newEnemy->Initialize();

			/// 左側
			// 座標設定
			Vector2 newPos = {offset.x - (enemySpaceOffset * i), offset.y + (enemySpaceOffset * i)};
			newEnemy->SetPosition(newPos);
			// 速さ設定
			newEnemy->SetVelocity(Vector2(0, -moveSpeed));
			// リストに追加
			enemys_.push_back(newEnemy);
		}
		for (int i = 1; i < ArrowNumber; i++) {
			// 生成・初期化
			Enemy* newEnemy = new Enemy();
			newEnemy->SetTexture(charaTex_);
			newEnemy->Initialize();

			/// 右側
			// 座標設定
			Vector2 newPos = {offset.x + (enemySpaceOffset * i), offset.y + (enemySpaceOffset * i)};
			newEnemy->SetPosition(newPos);
			// 速さ設定
			newEnemy->SetVelocity(Vector2(0, -moveSpeed));
			// リストに追加
			enemys_.push_back(newEnemy);
		}
		break;

	case kRight:
		// 左から右
		offset = {plPrevPos_.x - float(WinApp::kWindowWidth / 2), plPrevPos_.y};
		for (int i = 0; i < ArrowNumber; i++) {
			// 生成・初期化
			Enemy* newEnemy = new Enemy();
			newEnemy->SetTexture(charaTex_);
			newEnemy->Initialize();

			/// 左側
			// 座標設定
			Vector2 newPos = {offset.x - (enemySpaceOffset * i), offset.y - (enemySpaceOffset * i)};
			newEnemy->SetPosition(newPos);
			// 速さ設定
			newEnemy->SetVelocity(Vector2(moveSpeed, 0));
			// リストに追加
			enemys_.push_back(newEnemy);
		}
		for (int i = 1; i < ArrowNumber; i++) {
			// 生成・初期化
			Enemy* newEnemy = new Enemy();
			newEnemy->SetTexture(charaTex_);
			newEnemy->Initialize();

			/// 左側
			// 座標設定
			Vector2 newPos = {offset.x - (enemySpaceOffset * i), offset.y + (enemySpaceOffset * i)};
			newEnemy->SetPosition(newPos);
			// 速さ設定
			newEnemy->SetVelocity(Vector2(moveSpeed, 0));
			// リストに追加
			enemys_.push_back(newEnemy);
		}

		break;

	case kLeft:
		// 右から左
		offset = {plPrevPos_.x + float(WinApp::kWindowWidth / 2), plPrevPos_.y};
		for (int i = 0; i < ArrowNumber; i++) {
			// 生成・初期化
			Enemy* newEnemy = new Enemy();
			newEnemy->SetTexture(charaTex_);
			newEnemy->Initialize();

			/// 左側
			// 座標設定
			Vector2 newPos = {offset.x + (enemySpaceOffset * i), offset.y - (enemySpaceOffset * i)};
			newEnemy->SetPosition(newPos);
			// 速さ設定
			newEnemy->SetVelocity(Vector2(-moveSpeed, 0));
			// リストに追加
			enemys_.push_back(newEnemy);
		}
		for (int i = 1; i < ArrowNumber; i++) {
			// 生成・初期化
			Enemy* newEnemy = new Enemy();
			newEnemy->SetTexture(charaTex_);
			newEnemy->Initialize();

			/// 左側
			// 座標設定
			Vector2 newPos = {offset.x + (enemySpaceOffset * i), offset.y + (enemySpaceOffset * i)};
			newEnemy->SetPosition(newPos);
			// 速さ設定
			newEnemy->SetVelocity(Vector2(-moveSpeed, 0));
			// リストに追加
			enemys_.push_back(newEnemy);
		}

		break;

	}

}

void EnemyManager::ArrowBehaviorControl() 
{
	if (input_->TriggerKey(DIK_7)) {
		plPrevPos_ = playerPos_;
		this->ArrowBehavior(kDown);
		if (!isRespown)
			isRespown = true;
	}

	if (isRespown) {
		ArrowCoolTime++;
		if (ArrowCoolTime == 90) {
			this->ArrowBehavior(kUp);
		}
		if (ArrowCoolTime == 180) {
			plPrevPos_ = playerPos_;
			this->ArrowBehavior(kLeft);
		}
		if (ArrowCoolTime == 270) {
			this->ArrowBehavior(kRight);
			isRespown = false;
			ArrowCoolTime = 0;
		}
	}
}
