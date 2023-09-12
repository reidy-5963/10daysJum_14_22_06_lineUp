#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>
#include <cmath>
#include "Scroll.h"
#include "GlobalVariables.h"

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
	sceneNum = 1;
	gameTimer = setGameTime;

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 乱数
	unsigned int currentTime = (int)time(nullptr);
	srand(currentTime);

#pragma region プレイヤー
	// プレイヤー生成
	player_ = std::make_unique<Player>();
	player_->SetIsGameStart(true);
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
#pragma endregion

#pragma region 雑魚敵管理クラス

	enemyManager_ = EnemyManager::GetInstance();
	enemyManager_->Initialize();
#pragma endregion

#pragma region 背景
	backTex = TextureManager::Load("white1x1.png");
	back.reset(Sprite::Create(backTex, {0.0f, 0.0f}, {0.01f, 0.01f, 0.01f, 1.0f}, {0.0f, 0.0f}));
	Vector2 size = {1920 * 3, 1080 * 3};
	back->SetSize(size);
#pragma endregion 
	
#pragma region 背景
	numTex_ = TextureManager::Load("nums.png");
	numPos[0] = {1920.0f - 500.0f, 3.0f};
	numPos[1] = {numPos[0].x - 128.0f, numPos[0].y};

	for (int i = 0; i < 2; i++) {
		num[i].reset(Sprite::Create(numTex_, numPos[i], {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
		Vector2 tmpSize = {128.0f, 128.0f};
		num[i]->SetSize(tmpSize);
	}
#pragma endregion 

#pragma region 背景
	numTex_ = TextureManager::Load("nums.png");
	enemyNumPos[0] = {500.0f - 500.0f, 3.0f};
	enemyNumPos[1] = {enemyNumPos[0].x - 128.0f, enemyNumPos[0].y};

	for (int i = 0; i < 2; i++) {
		enemyNum[i].reset(
		    Sprite::Create(numTex_, enemyNumPos[i], {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
		Vector2 tmpSize = {128.0f, 128.0f};
		enemyNum[i]->SetSize(tmpSize);
	}
#pragma endregion 

	InitializeGrobalVariables();
	BGMHandle_ = Audio::GetInstance()->LoadWave("GameScene.wav");
}

/// <summary>
/// 毎フレーム処理
/// </summary>
void GameScene::Update() { 
	ApplyGrobalVariables();
	enemyNumPos[0] = {enemyNumPos_.x - 64.0f, enemyNumPos_.y};
	enemyNumPos[1] = {enemyNumPos_.x + 64.0f, enemyNumPos_.y};
	numPos[0] = {scoreNumPos_.x - 64.0f, scoreNumPos_.y};
	numPos[1] = {scoreNumPos_.x + 64.0f, scoreNumPos_.y};

	// BGM再生
	if (audio_->IsPlaying(BGMHandle_) == 0 || BGMHandle_ == -1) {
		BGMHandle_ = audio_->PlayWave(BGMHandle_, true, bolume);
	}

	if (!isGameSet_) {
	// スクロールの更新処理
	//Scroll* scroll = Scroll::GetInstance();
	scroll_->Update();

	// 敵の更新処理
	enemyManager_->SetPlayer(player_->GetPosition());
	enemyManager_->SetSceneVelo(sceneShakevelo_);
	enemyManager_->Update();

	// プレイヤーの更新処理
	player_->Update();

	// 当たり判定
	CheckAllCollision();

	// 画面の揺れ更新処理
	MyMath::ShakeUpdate(sceneShakevelo_, issceneShake, sceneaAmplitNum);
	player_->SetSceneVelo(sceneShakevelo_);
	boss_->SetSceneVelo(sceneShakevelo_);

	if (killCount_ > setKillCount && !isBossRespown_) {
		isBossRespown_ = true;
		boss_->RespownBoss();
	}

	// ボスの更新処理
		boss_->SetPlayer(player_->GetPosition());
		boss_->Update();
	

		if (--gameTimer < 0) {
			isGameSet_ = true;
		}
	
	// 背景の更新処理
	}
	//
	else if (isGameSet_) {
		
		
	}
	ImGui::Begin("timer");
	ImGui::Text("%d", gameTimer);
	ImGui::Text("timer : %d", gameTimer / 60);
	ImGui::Text("ten : %d", (gameTimer / 60) % 10);
	ImGui::End();

	int timeraaa = gameTimer / 60;
	Tn(scoreTen, scoreOne, timeraaa);
	num[0]->SetTextureRect({0.0f + (scoreTen * 128.0f), 0.0f}, {128.0f, 128.0f});
	num[1]->SetTextureRect({0.0f + (scoreOne * 128.0f), 0.0f}, {128.0f, 128.0f});

	num[0]->SetPosition(numPos[0]);
	num[1]->SetPosition(numPos[1]);

	timeraaa = setKillCount - killCount_;
	Tn(enemyTen, enemyOne, timeraaa);
	enemyNum[0]->SetTextureRect({0.0f + (enemyTen * 128.0f), 0.0f}, {128.0f, 128.0f});
	enemyNum[1]->SetTextureRect({0.0f + (enemyOne * 128.0f), 0.0f}, {128.0f, 128.0f});

	enemyNum[0]->SetPosition(enemyNumPos[0]);
	enemyNum[1]->SetPosition(enemyNumPos[1]);


	if (gameTimer > 60 * 100) {
		gameTimer = 60 * 99;
	}


	pickUpTailTime = 60 * setTailTime;
	funnelDamage = 60 * setTailTime;
	eneBulletDamage = 60 * setEneBulletDamage;
	bossEnemyDamage = 60 * setBossEnemyDamage;
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
	player_->DrawUI();

	num[0]->Draw();

	num[1]->Draw();

	if (!boss_->IsAlive()) {
		enemyNum[0]->Draw();
		enemyNum[1]->Draw();

	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::InitializeGrobalVariables() { // グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "GameScene";

	// 作ったグループにそれぞれアイテムを追加
	gloVars->CreateGroup(groupName);
	gloVars->AddItem(groupName, "enemyNumPos_", enemyNumPos_);
	gloVars->AddItem(groupName, "scoreNumPos_", scoreNumPos_);

	gloVars->AddItem(groupName, "setTailTime", setTailTime);
	gloVars->AddItem(groupName, "setFunnelDamage", setFunnelDamage);
	gloVars->AddItem(groupName, "setEneBulletDamage", setEneBulletDamage);
	gloVars->AddItem(groupName, "setBossEnemyDamage", setBossEnemyDamage);
}

void GameScene::ApplyGrobalVariables() { // グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "GameScene";
	// 作ったグループにあるアイテムから値を取得
	enemyNumPos_ = gloVars->GetVector2Value(groupName, "enemyNumPos_");
	scoreNumPos_ = gloVars->GetVector2Value(groupName, "scoreNumPos_");

	setTailTime = gloVars->GetIntValue(groupName, "setTailTime");
	setFunnelDamage = gloVars->GetIntValue(groupName, "setFunnelDamage");
	setEneBulletDamage = gloVars->GetIntValue(groupName, "setEneBulletDamage");
	setBossEnemyDamage = gloVars->GetIntValue(groupName, "setBossEnemyDamage");
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
					if (player_->GetTail() >= 6) {
						gameTimer += pickUpTailTime;
					}

					enemy->SetIsDead(true);
				}
				if (!enemy->IsParasite()) {
					player_->OnCollision();
					issceneShake = true;
					sceneaAmplitNum = 40;
					gameTimer -= eneBulletDamage;
					enemy->SetIsDead(true);
				}
				//
				//killCount_ += 1;

			} 
			//
			else if (player_->GetIsInvisible()) {
				if (enemy->IsParasite()) {
					gameTimer += pickUpTailTime;

					player_->AddTails();
					enemy->SetIsDead(true);
				}
			}
		}
	}
#pragma endregion

#pragma region ボスの弾とプレイヤー

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
			gameTimer -= eneBulletDamage;
			player_->OnCollision();
			issceneShake = true;
			sceneaAmplitNum = 40;
		}
	}

#pragma endregion

#pragma region ボスのファンネルとプレイヤー

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
			gameTimer -= funnelDamage;
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
					//enemy->OnCollision();
					//tail->OnCollision();
					//killCount_ += 1;

				}
			}
		}
	}
#pragma endregion

#pragma region ボスのファンネルと尻尾
	//for (BossFunnel* bossFunnel_ : bossFunnel) {
	//	// エネミーの位置取得
	//	targetA = bossFunnel_->GetPosition();
	//	for (Tail* tail : tails) {
	//		targetB = tail->GetPosition();
	//		float distance = std::sqrtf(
	//		    std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));
	//		float radius = tail->GetRadius() + bossFunnel_->GetRadius();
	//		// 交差判定
	//		if (distance <= radius) {
	//			// コールバック
	//			bossFunnel_->OnCollision();
	//			tail->OnCollision();
	//		}
	//	}
	//}
#pragma endregion

#pragma region ボスの弾と尻尾
	//for (BossBullet* bossBullet_ : bossBullet) {
	//	// エネミーの位置取得
	//	targetA = bossBullet_->GetPosition();
	//	for (Tail* tail : tails) {
	//		targetB = tail->GetPosition();
	//		float distance = std::sqrtf(
	//		    std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));
	//		float radius = tail->GetRadius() + bossBullet_->GetRadius();
	//		// 交差判定
	//		if (distance <= radius) {
	//			// コールバック
	//			bossBullet_->OnCollision();
	//			tail->OnCollision();
	//		}
	//	}
	//}
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
			float bulletRadius = playerBullet_->GetRadius();

			if (distance <= radius) {
				// コールバック
				if (!enemy->IsParasite()) {
					if (!playerBullet_->IsCollapse()) {
						enemy->OnCollision();
						killCount_ += 1;
					}
					if (bulletRadius < 32.0f) {
						playerBullet_->OnCollision();
					}
				}

			
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

		float bulletRadius = playerBullet_->GetRadius();

		if (distance <= radius && boss_->IsAlive()) {
			if (!playerBullet_->IsCollapse()) {
				// コールバック
				playerBullet_->OnCollision();
				boss_->OnCollision();
				if (bulletRadius == 64.0f / 1) {
					boss_->SetHp(boss_->GetHp() - 21);
				} else if (bulletRadius == 64.0f / 2) {
					boss_->SetHp(boss_->GetHp() - 15);
				} else if (bulletRadius == 64.0f / 3) {
					boss_->SetHp(boss_->GetHp() - 10);
				} else if (bulletRadius == 64.0f / 4) {
					boss_->SetHp(boss_->GetHp() - 6);
				} else if (bulletRadius == 64.0f / 5) {
					boss_->SetHp(boss_->GetHp() - 3);
				} else if (bulletRadius == 64.0f / 6) {
					boss_->SetHp(boss_->GetHp() - 1);
				}
			}
			
		}
	}
#pragma endregion

#pragma region プレイヤーとボス 
	targetA = player_->GetPosition();
	targetB = boss_->GetPosition();

	float distance = std::sqrtf(
			    std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));

	float radius = player_->GetRadius() + boss_->GetRadius();

	// 交差判定
	if (distance <= radius && boss_->IsAlive()) {
		if (!player_->GetIsInvisible()) {
			// コールバック
			player_->OnCollision();
			gameTimer -= bossEnemyDamage;
			issceneShake = true;
			sceneaAmplitNum = 40;
			//boss_->OnCollision();
		}
	}
#pragma endregion
}
