#include "EnemyManager.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <time.h>
#include <stdlib.h>
#include "WinApp.h"
#include <cmath>
#include <numbers>
#include "MyMath.h"
#include "Audio.h"

EnemyManager* EnemyManager::GetInstance() 
{
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize() { 
	input_ = Input::GetInstance();

	charaTex_ = TextureManager::Load("Enemy_ver2.png");
	parasiteTex_ = TextureManager::Load("Parasite.png");
	particleTex_ = TextureManager::Load("Particle.png");
	collapseTex_ = TextureManager::Load("greenTailbreak.png");

	deadSEHandel_ = Audio::GetInstance()->LoadWave("music/EnemyDead.mp3");

	enemys_.clear();

}

void EnemyManager::Update() 
{
#ifdef _DEBUG
	ImGui::Begin("count");
	ImGui::Text("isRespown : %d", patternInterval_);
	ImGui::Text("arrowCoolTime : %d", kInterval_);
	ImGui::End();

#endif // _DEBUG

	normalSpawnTimer_++;
	// ボス出現中
	if (isBossAlive_) {
		autoSpawnSecond_ = 8;
	} 
	// ボス出現前
	else {
		autoSpawnSecond_ = 3;
		FourPointsSpawn();
	}
	if (!isArrowRespown_) {
		patternInterval_++;
	}
	int maxInterVal = 13;
	int minInterVal = 9;
	// 尻尾の数が最大の場合
	if (tailSize_ == 6) {
		kIntervalSecond_ = maxInterVal;
	}
	// それ以外
	else {
		kIntervalSecond_ = minInterVal;
	}

	FormationSpawnUpdate();

	ArrowBehaviorControl();

	EnemyUpdate();
}

void EnemyManager::Draw() 
{
	// エネミーの描画処理
	for (Enemy* enemy : enemys_) {
		enemy->Draw();
	}
}

void EnemyManager::EnemyUpdate() 
{
	// 消去処理
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	// エネミーの更新
	for (Enemy* enemy : enemys_) {
		enemy->SetIsBossParasite(isBossParasite);
		enemy->SetSceneVelo(sceneVelo_);
		enemy->Update();
	}
}

void EnemyManager::FormationSpawnUpdate() 
{
	if (patternInterval_ == kInterval_) {
		int random = rand() % 6;
		patternInterval_ = 0;
		switch (random) {
		case 0:
			DiagonalBehavior();
			break;
		case 1:
			DiagonalClockWiseBehavior();
			break;
		case 2:
			ArrowBehaviorPlay();
			break;
		case 3:
			DiagonalBehavior();
			DiagonalClockWiseBehavior();
			break;
		case 4:
			ArrowBehaviorPlay();
			VerticalSpawn();
			break;
		case 5:
			ArrowBehaviorPlay();
			HorizontalSpawn();
			break;
		}
	}


}

void EnemyManager::CreateEnemy(int spownPoint) {
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
	newEnemy->SetVelocity(RandomRadianVector());
	newEnemy->SetParasiteTexture(parasiteTex_);
	newEnemy->SetParticleTex(particleTex_);
	newEnemy->SetCollapseTexture(collapseTex_);

	enemys_.push_back(newEnemy);
	
}

void EnemyManager::AddEnemy(const Vector2& position, const Vector2& velocity) {
	Enemy* newEnemy = new Enemy();
	newEnemy->SetTexture(charaTex_);
	newEnemy->Initialize();
	Vector2 pos = position;
	Vector2 velo = velocity;
	newEnemy->SetPosition(pos);
	newEnemy->SetVelocity(velo);
	newEnemy->SetParasiteTexture(parasiteTex_);
	newEnemy->SetParticleTex(particleTex_);
	newEnemy->SetCollapseTexture(collapseTex_);
	newEnemy->SetSEHandle(deadSEHandel_);

	enemys_.push_back(newEnemy);
}

void EnemyManager::AddEnemy(
    const Vector2& position, const Vector2& velocity, const Vector2& direction) 
{
	Enemy* newEnemy = new Enemy();
	newEnemy->SetTexture(charaTex_);
	newEnemy->Initialize();
	Vector2 pos = position;
	Vector2 velo = velocity;
	newEnemy->SetPosition(pos);
	newEnemy->SetVelocity(velo);
	newEnemy->SetRotation(std::atan2(direction.y,direction.x));
	newEnemy->SetParasiteTexture(parasiteTex_);
	newEnemy->SetParticleTex(particleTex_);
	newEnemy->SetSEHandle(deadSEHandel_);

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

void EnemyManager::FourPointsSpawn() 
{ 
	int spRadius = 151;
	int point = rand() % spRadius - 50;

	Vector2 spawnPoint = {};
	Vector2 ankerPoint = playerPos_;

	if (normalSpawnTimer_ > kRespownTimer_) {
		spawnShiftFrame_++;
		if (spawnShiftFrame_ == 10) {
			spawnPoint = {
			    ankerPoint.x - float(WinApp::kWindowWidth / 2 + float(point)),
			    ankerPoint.y - float(WinApp::kWindowHeight / 2) + float(point)};
			AddEnemy(spawnPoint, RandomRadianVector());

		} else if (spawnShiftFrame_ == 20) {
			spawnPoint = {
			    ankerPoint.x - float(WinApp::kWindowWidth + float(point)),
			    ankerPoint.y + float(WinApp::kWindowHeight / 2) + float(point)};
			AddEnemy(spawnPoint, RandomRadianVector());

		} else if (spawnShiftFrame_ == 30) {
			spawnPoint = {
			    ankerPoint.x + float(WinApp::kWindowWidth / 2) + float(point),
			    ankerPoint.y - float(WinApp::kWindowHeight / 2) + float(point)};
			AddEnemy(spawnPoint, RandomRadianVector());

		} else if (spawnShiftFrame_ == 40) {
			spawnPoint = {
			    ankerPoint.x + float(WinApp::kWindowWidth / 2) + float(point),
			    ankerPoint.y + float(WinApp::kWindowHeight / 2) + float(point)};
			AddEnemy(spawnPoint, RandomRadianVector());

			spawnShiftFrame_ = 0;
			normalSpawnTimer_ = 0;
		}
	}

}

void EnemyManager::RushSpawn() 
{ 
	float speedPower = 0.4f;
	float ankerOffset = 300.0f;
	Vector2 ankerPoint = bossPos_;
	Vector2 leftTop = {ankerPoint.x - ankerOffset, ankerPoint.y - ankerOffset};
	Vector2 rightTop = {ankerPoint.x + ankerOffset, ankerPoint.y - ankerOffset};
	Vector2 leftBottom = {ankerPoint.x - ankerOffset, ankerPoint.y + ankerOffset};
	Vector2 rightBottom = {ankerPoint.x + ankerOffset, ankerPoint.y + ankerOffset};

	AddEnemy(leftTop, Vector2(-speedPower, -speedPower));
	AddEnemy(rightTop, Vector2(speedPower, -speedPower));
	AddEnemy(leftBottom, Vector2(-speedPower, speedPower));
	AddEnemy(rightBottom, Vector2(speedPower, speedPower));
}

void EnemyManager::StartSpawn() {
	Vector2 spawnPoint = {};
	float offset = 100.0f;
	Vector2 ankerPoint = {float(WinApp::kWindowWidth / 2), float(WinApp::kWindowHeight / 2) + (offset * 3)};
	// 右真ん中
	spawnPoint = Vector2(ankerPoint.x + float(WinApp::kWindowWidth / 2), ankerPoint.y);
	AddEnemy(spawnPoint, MyMath::Normalize(spawnPoint - ankerPoint));

	// 右下
	spawnPoint = Vector2(
	    ankerPoint.x + float(WinApp::kWindowWidth / 2),
	    ankerPoint.y + float(WinApp::kWindowHeight / 2));
	AddEnemy(spawnPoint, MyMath::Normalize(spawnPoint - ankerPoint));

	// 右上
	spawnPoint = Vector2(
	    ankerPoint.x + float(WinApp::kWindowWidth / 2),
	    ankerPoint.y - float(WinApp::kWindowHeight / 2));
	AddEnemy(spawnPoint, MyMath::Normalize(spawnPoint - ankerPoint));

	// 下
	spawnPoint= Vector2(
	    ankerPoint.x + float(WinApp::kWindowWidth / 4),
	    ankerPoint.y + float(WinApp::kWindowHeight / 2) + offset);
	AddEnemy(spawnPoint, MyMath::Normalize(spawnPoint - ankerPoint));

	// 上
	spawnPoint = Vector2(
	    ankerPoint.x + float(WinApp::kWindowWidth / 4),
	    ankerPoint.y - float(WinApp::kWindowHeight / 2) - offset);
	AddEnemy(spawnPoint, MyMath::Normalize(spawnPoint - ankerPoint));
}

void EnemyManager::DiagonalBehavior() 
{ 
	float enemySpaceOffset = 75.0f;

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
		newEnemy->SetVelocity(Vector2(norm.x * moveSpeed, norm.y * moveSpeed));
		// SEの設定
		newEnemy->SetSEHandle(deadSEHandel_);

		newEnemy->SetParasiteTexture(parasiteTex_);
		newEnemy->SetParticleTex(particleTex_);
		newEnemy->SetCollapseTexture(collapseTex_);

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
		// SEの設定
		newEnemy->SetSEHandle(deadSEHandel_);

		newEnemy->SetParasiteTexture(parasiteTex_);
		newEnemy->SetParticleTex(particleTex_);
		newEnemy->SetCollapseTexture(collapseTex_);

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
		// SEの設定
		newEnemy->SetSEHandle(deadSEHandel_);

		newEnemy->SetParasiteTexture(parasiteTex_);
		newEnemy->SetParticleTex(particleTex_);
		newEnemy->SetCollapseTexture(collapseTex_);

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
		// SEの設定
		newEnemy->SetSEHandle(deadSEHandel_);

		newEnemy->SetParasiteTexture(parasiteTex_);
		newEnemy->SetParticleTex(particleTex_);
		newEnemy->SetCollapseTexture(collapseTex_);

		// リストに追加
		enemys_.push_back(newEnemy);
	}
}

void EnemyManager::DiagonalClockWiseBehavior() 
{
	float enemySpaceOffset = 75.0f;

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
		// SEの設定
		newEnemy->SetSEHandle(deadSEHandel_);

		newEnemy->SetParasiteTexture(parasiteTex_);
		newEnemy->SetParticleTex(particleTex_);
		newEnemy->SetCollapseTexture(collapseTex_);

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
		// SEの設定
		newEnemy->SetSEHandle(deadSEHandel_);

		newEnemy->SetParasiteTexture(parasiteTex_);
		newEnemy->SetParticleTex(particleTex_);
		newEnemy->SetCollapseTexture(collapseTex_);

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
		// SEの設定
		newEnemy->SetSEHandle(deadSEHandel_);

		newEnemy->SetParasiteTexture(parasiteTex_);
		newEnemy->SetParticleTex(particleTex_);
		newEnemy->SetCollapseTexture(collapseTex_);

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
		// SEの設定
		newEnemy->SetSEHandle(deadSEHandel_);

		newEnemy->SetParasiteTexture(parasiteTex_);
		newEnemy->SetParticleTex(particleTex_);
		newEnemy->SetCollapseTexture(collapseTex_);

		// リストに追加
		enemys_.push_back(newEnemy);
	}
}

void EnemyManager::ArrowBehavior(int switchPatt) 
{
	// 敵同士の間隔
	float enemySpaceOffset = 100.0f;
	// 数
	int ArrowNumber = 3;
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
			// SEの設定
			newEnemy->SetSEHandle(deadSEHandel_);

			newEnemy->SetParasiteTexture(parasiteTex_);
			newEnemy->SetParticleTex(particleTex_);
			newEnemy->SetCollapseTexture(collapseTex_);

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
			// SEの設定
			newEnemy->SetSEHandle(deadSEHandel_);

			newEnemy->SetParasiteTexture(parasiteTex_);
			newEnemy->SetParticleTex(particleTex_);
			newEnemy->SetCollapseTexture(collapseTex_);

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
			// SEの設定
			newEnemy->SetSEHandle(deadSEHandel_);

			newEnemy->SetParasiteTexture(parasiteTex_);
			newEnemy->SetParticleTex(particleTex_);
			newEnemy->SetCollapseTexture(collapseTex_);

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
			// SEの設定
			newEnemy->SetSEHandle(deadSEHandel_);

			newEnemy->SetParasiteTexture(parasiteTex_);
			newEnemy->SetParticleTex(particleTex_);
			newEnemy->SetCollapseTexture(collapseTex_);

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
			// SEの設定
			newEnemy->SetSEHandle(deadSEHandel_);

			newEnemy->SetParasiteTexture(parasiteTex_);
			newEnemy->SetParticleTex(particleTex_);
			newEnemy->SetCollapseTexture(collapseTex_);

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
			// SEの設定
			newEnemy->SetSEHandle(deadSEHandel_);

			newEnemy->SetParasiteTexture(parasiteTex_);
			newEnemy->SetParticleTex(particleTex_);
			newEnemy->SetCollapseTexture(collapseTex_);

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
			// SEの設定
			newEnemy->SetSEHandle(deadSEHandel_);

			newEnemy->SetParasiteTexture(parasiteTex_);
			newEnemy->SetParticleTex(particleTex_);
			newEnemy->SetCollapseTexture(collapseTex_);

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
			// SEの設定
			newEnemy->SetSEHandle(deadSEHandel_);

			newEnemy->SetParasiteTexture(parasiteTex_);
			newEnemy->SetParticleTex(particleTex_);
			newEnemy->SetCollapseTexture(collapseTex_);

			// リストに追加
			enemys_.push_back(newEnemy);
		}

		break;

	}

}

void EnemyManager::ArrowBehaviorControl() 
{
	if (isArrowRespown_) {
		ArrowCoolTime++;
		if (ArrowCoolTime == kArrowDelay_ * 1) {
			this->ArrowBehavior(kUp);
		}
		if (ArrowCoolTime == kArrowDelay_ * 2) {
			plPrevPos_ = playerPos_;
			this->ArrowBehavior(kLeft);
		}
		if (ArrowCoolTime == kArrowDelay_ * 3) {
			this->ArrowBehavior(kRight);
			isArrowRespown_ = false;
			ArrowCoolTime = 0;
		}
	}
}

void EnemyManager::ArrowBehaviorPlay() 
{
	if (!isArrowRespown_) {
		plPrevPos_ = playerPos_;
		isArrowRespown_ = true;
		ArrowBehavior(kDown);
	}
}

void EnemyManager::VerticalSpawn() 
{ 
	// 左側だったら
	if (playerPos_.x < areaLeft2Top.x) {
		// mid
		AddEnemy(
		    Vector2(areaMid2Mid.x - scWidthHalfSize, areaLeft2Top.y - scHeightHalfSize)
			, Vector2(-1.0f, 0.0f));
		AddEnemy(
		    Vector2(areaMid2Mid.x - scWidthHalfSize, areaMid2Mid.y - scHeightHalfSize)
			, Vector2(-1.0f, 0.0f));
		AddEnemy(
		    Vector2(areaMid2Mid.x - scWidthHalfSize, areaRight2Bottom.y - scHeightHalfSize)
			, Vector2(-1.0f, 0.0f));
		// right
		AddEnemy(
		    Vector2(areaRight2Bottom.x - scWidthHalfSize, areaLeft2Top.y - scHeightHalfSize),
		    Vector2(-1.0f, 0.0f));
		AddEnemy(
		    Vector2(areaRight2Bottom.x - scWidthHalfSize, areaMid2Mid.y - scHeightHalfSize),
		    Vector2(-1.0f, 0.0f));
		AddEnemy(
		    Vector2(areaRight2Bottom.x - scWidthHalfSize, areaRight2Bottom.y - scHeightHalfSize),
		    Vector2(-1.0f, 0.0f));

	} 
	// 中央だったら
	else if (playerPos_.x >= areaLeft2Top.x && playerPos_.x < areaMid2Mid.x) {
		AddEnemy(
		    Vector2(areaLeft2Top.x - scWidthHalfSize, areaLeft2Top.y - scHeightHalfSize),
		    Vector2(-1.0f, 0.0f));
		AddEnemy(
		    Vector2(areaLeft2Top.x - scWidthHalfSize, areaMid2Mid.y - scHeightHalfSize),
		    Vector2(-1.0f, 0.0f));
		AddEnemy(
		    Vector2(areaLeft2Top.x - scWidthHalfSize, areaRight2Bottom.y - scHeightHalfSize),
		    Vector2(-1.0f, 0.0f));

		AddEnemy(
		    Vector2(areaRight2Bottom.x - scWidthHalfSize, areaLeft2Top.y - scHeightHalfSize),
		    Vector2(-1.0f, 0.0f));
		AddEnemy(
		    Vector2(areaRight2Bottom.x - scWidthHalfSize, areaMid2Mid.y - scHeightHalfSize),
		    Vector2(-1.0f, 0.0f));
		AddEnemy(
		    Vector2(areaRight2Bottom.x - scWidthHalfSize, areaRight2Bottom.y - scHeightHalfSize),
		    Vector2(-1.0f, 0.0f));
	} 
	// 右側だったら
	else {
		AddEnemy(
		    Vector2(areaLeft2Top.x - scWidthHalfSize, areaLeft2Top.y - scHeightHalfSize),
		    Vector2(1.0f, 0.0f));
		AddEnemy(
		    Vector2(areaLeft2Top.x - scWidthHalfSize, areaMid2Mid.y - scHeightHalfSize),
		    Vector2(1.0f, 0.0f));
		AddEnemy(
		    Vector2(areaLeft2Top.x - scWidthHalfSize, areaRight2Bottom.y - scHeightHalfSize),
		    Vector2(1.0f, 0.0f));

		AddEnemy(
		    Vector2(areaMid2Mid.x - scWidthHalfSize, areaLeft2Top.y - scHeightHalfSize),
		    Vector2(1.0f, 0.0f));
		AddEnemy(
		    Vector2(areaMid2Mid.x - scWidthHalfSize, areaMid2Mid.y - scHeightHalfSize),
		    Vector2(1.0f, 0.0f));
		AddEnemy(
		    Vector2(areaMid2Mid.x - scWidthHalfSize, areaRight2Bottom.y - scHeightHalfSize),
		    Vector2(1.0f, 0.0f));
	}
}

void EnemyManager::HorizontalSpawn() 
{

	// 上側だったら
	if (playerPos_.y < areaLeft2Top.y) {
		AddEnemy(
		    Vector2(areaLeft2Top.x - scWidthHalfSize, areaMid2Mid.y - scHeightHalfSize),
		    Vector2(0.0f, -1.0f));
		AddEnemy(
		    Vector2(areaMid2Mid.x - scWidthHalfSize, areaMid2Mid.y - scHeightHalfSize),
		    Vector2(0.0f, -1.0f));
		AddEnemy(
		    Vector2(areaRight2Bottom.x - scWidthHalfSize, areaMid2Mid.y - scHeightHalfSize),
		    Vector2(0.0f, -1.0f));

		AddEnemy(
		    Vector2(areaLeft2Top.x - scWidthHalfSize, areaRight2Bottom.y - scHeightHalfSize),
		    Vector2(0.0f, -1.0f));
		AddEnemy(
		    Vector2(areaMid2Mid.x - scWidthHalfSize, areaRight2Bottom.y - scHeightHalfSize),
		    Vector2(0.0f, -1.0f));
		AddEnemy(
		    Vector2(areaRight2Bottom.x - scWidthHalfSize, areaRight2Bottom.y - scHeightHalfSize),
		    Vector2(0.0f, -1.0f));

	}
	// 中央だったら
	else if (playerPos_.x >= areaLeft2Top.y && playerPos_.x < areaMid2Mid.y) {
		AddEnemy(
		    Vector2(areaLeft2Top.x - scWidthHalfSize, areaLeft2Top.y - scHeightHalfSize),
		    Vector2(0.0f, 1.0f));
		AddEnemy(
		    Vector2(areaMid2Mid.x - scWidthHalfSize, areaLeft2Top.y - scHeightHalfSize),
		    Vector2(0.0f, 1.0f));
		AddEnemy(
		    Vector2(areaRight2Bottom.x - scWidthHalfSize, areaLeft2Top.y - scHeightHalfSize),
		    Vector2(0.0f, 1.0f));

		AddEnemy(
		    Vector2(areaLeft2Top.x - scWidthHalfSize, areaRight2Bottom.y - scHeightHalfSize),
		    Vector2(0.0f, -1.0f));
		AddEnemy(
		    Vector2(areaMid2Mid.x - scWidthHalfSize, areaRight2Bottom.y - scHeightHalfSize),
		    Vector2(0.0f, -1.0f));
		AddEnemy(
		    Vector2(areaRight2Bottom.x - scWidthHalfSize, areaRight2Bottom.y - scHeightHalfSize),
		    Vector2(0.0f, -1.0f));
	}
	// 下側だったら
	else {
		AddEnemy(
		    Vector2(areaLeft2Top.x - scWidthHalfSize, areaLeft2Top.y - scHeightHalfSize),
		    Vector2(0.0f, 1.0f));
		AddEnemy(
		    Vector2(areaMid2Mid.x - scWidthHalfSize, areaLeft2Top.y - scHeightHalfSize),
		    Vector2(0.0f, 1.0f));
		AddEnemy(
		    Vector2(areaRight2Bottom.x - scWidthHalfSize, areaLeft2Top.y - scHeightHalfSize),
		    Vector2(0.0f, 1.0f));

		AddEnemy(
		    Vector2(areaLeft2Top.x - scWidthHalfSize, areaMid2Mid.y - scHeightHalfSize),
		    Vector2(0.0f, 1.0f));
		AddEnemy(
		    Vector2(areaMid2Mid.x - scWidthHalfSize, areaMid2Mid.y - scHeightHalfSize),
		    Vector2(0.0f, 1.0f));
		AddEnemy(
		    Vector2(areaRight2Bottom.x - scWidthHalfSize, areaMid2Mid.y - scHeightHalfSize),
		    Vector2(0.0f, 1.0f));
	}
}
