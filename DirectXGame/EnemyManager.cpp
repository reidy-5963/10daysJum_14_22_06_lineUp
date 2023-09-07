#include "EnemyManager.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <time.h>
#include <stdlib.h>

EnemyManager* EnemyManager::GetInstance() 
{
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize() { 
	enemyTexture_ = TextureManager::Load("Enemy.png");

}

void EnemyManager::Update() 
{
	// 消去処理
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	TentRes();

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
		newEnemy->SetTexture(this->enemyTexture_);
		newEnemy->Initialize();
		Vector2 res = {
		    float(rand() % 1280 + newEnemy->GetRadius()),
		    float(rand() % 720 + newEnemy->GetRadius())};
		newEnemy->SetPosition(res);
		enemys_.push_back(newEnemy);
		respownCount = kRespownTimer;
	}
}
