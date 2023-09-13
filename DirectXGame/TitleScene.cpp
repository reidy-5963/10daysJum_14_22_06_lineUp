#include "TitleScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>
#include <cmath>
#include "GlobalVariables.h"

/// <summary>
/// コンストクラタ
/// </summary>
TitleScene::TitleScene() {}

/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene() {}

void TitleScene::Initialize() {
	// このシーンはタイトル
	sceneNum = TITLE;
	// 静的変数の初期化
	StaticValueInitialize();
	BGMHandle_ = Audio::GetInstance()->LoadWave("music/GameScene.wav");

	Scroll::GetInstance()->Initialize();

	player_.release();
	
	title2gameSceneAria_.release();
	titleLogo_.release();

	// いろいろ
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	aria_t_ = 0.0f;
	isAriaMove_ = false;
	titleLogo_t_ = 0.0f;
	titleLogoMove_ = false;
	TitleTutrialStart = false;

#pragma region シーン遷移エリア
	ariaTex_ = TextureManager::Load("T2GaSceneAria.png");
	title2gameSceneAria_.reset(Sprite::Create(ariaTex_, ariaPos_, ariaColor_, {0.0f, 0.0f}));
	ariaPos_ = {1800.0f, 0.0f};

#pragma endregion

#pragma region 背景
	backTex = TextureManager::Load("white1x1.png");
	back.reset(Sprite::Create(backTex, {0.0f, 0.0f}, {0.01f, 0.01f, 0.01f, 1.0f}, {0.0f, 0.0f}));
	Vector2 size = {1920, 1080};
	back->SetSize(size);

#pragma endregion

#pragma region タイトル 
	// タイトルロゴの位置
	titleLogo_pos_ = {WinApp::kWindowWidth / 2, (WinApp::kWindowHeight / 2) / 2};
	
	// 線形補完用スタート位置
	titleLogoStartPos_ = {WinApp::kWindowWidth / 2, (WinApp::kWindowHeight / 2) / 2};
	
	// 線形補完用終わり位置
	titleLogoEndPos_ = {WinApp::kWindowWidth / 2, (WinApp::kWindowHeight / 2) / 2 - 10.0f};
	
	// タイトルロゴのテクスチャ読み込み
	titleLogo_tex_ = TextureManager::Load("Title_ver2.png");
	
	// タイトルロゴのスプライト生成
	titleLogo_.reset(Sprite::Create(titleLogo_tex_, titleLogo_pos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

#pragma endregion

#pragma region プレイヤー
	// プレイヤー生成
	player_ = std::make_unique <Player>();
	player_->SetIsGameStart(false);
	// プレイヤーの初期化処理
	player_->Initialize();
#pragma endregion

	enemyManager_ = EnemyManager::GetInstance();
	enemyManager_->Initialize();
	enemyManager_->SetIsGameMode(false);

	title2gameSceneAria_->SetPosition(ariaStartPos_);

	// グローバル変数の初期化処理
	InitializeGrobalVariables();
	ApplyGrobalVariables();

	for (int i = 0; i < 6; i++) {
		enemyManager_->AddEnemy(enemyEndPos_[i], {0.0f, 0.0f});
	}
}

void TitleScene::StaticValueInitialize() {
	resultTime = 0;
	resultScore = 0;
	resultKillCount = 0;
	isBossDead = false;
	is6Tails = false;
}

void TitleScene::Update() { 
	// BGM再生
	if (audio_->IsPlaying(BGMHandle_) == 0 || BGMHandle_ == -1) {
		BGMHandle_ = audio_->PlayWave(BGMHandle_, true, bolume);
	}

	ApplyGrobalVariables();


	titleLogo_->SetPosition(titleLogo_pos_);

	// プレイヤーの更新処理
	player_->Update();


	if (!TitleTutrialStart) {

		NotTutorialUpdate();
	}
	//
	else if (TitleTutrialStart) {

		TutorialUpdate();
	}

	CheckAllCollision();
}

void TitleScene::NotTutorialUpdate() {

	if (!titleLogoMove_) {
		titleLogo_pos_ = MyMath::EaseInCubicF(titleLogo_t_, titleLogoStartPos_, titleLogoEndPos_);
		MyMath::CountT(titleLogo_t_, 0.0f, titleLogoMove_, true, 0.1f);
	}
	if (titleLogoMove_) {
		titleLogo_pos_ = MyMath::EaseInCubicF(titleLogo_t_, titleLogoEndPos_, titleLogoStartPos_);
		MyMath::CountT(titleLogo_t_, 0.0f, titleLogoMove_, false, 0.1f);
		if (!titleLogoMove_) {
			titleLogo_pos_ =
			    MyMath::EaseInCubicF(titleLogo_t_, titleLogoStartPos_, titleLogoEndPos_);
			MyMath::CountT(titleLogo_t_, 0.0f, titleLogoMove_, true, 0.1f);
		}
	}

	if (input_->IsTriggerMouse(0)) {
		TitleTutrialStart = true;
	}
}

void TitleScene::TutorialUpdate() {
	if (!isAriaMove_) {
		ariaPos_ = MyMath::lerp(aria_t_, ariaStartPos_, ariaEndPos_);
		MyMath::CountT(aria_t_, 1.0f, isAriaMove_, true, 0.05f);
	}
	title2gameSceneAria_->SetColor(ariaColor_);
	title2gameSceneAria_->SetPosition(ariaPos_);

	if (!titleLogoMove_) {
		titleLogoMove_ = true;
	}
	if (titleLogoMove_) {
		titleLogo_pos_ =
		    MyMath::EaseInCubicF(titleLogo_t_, titleLogoEndPos_, {titleLogoEndPos_.x, -200.0f});
		MyMath::CountT(titleLogo_t_, 1.0f, titleLogoMove_, true, 0.1f);
	}

	enemyManager_->SetPlayer(player_->GetPosition());
	enemyManager_->SetTailSize(player_->GetTail());
	enemyManager_->EnemyUpdate();

	if (player_->GetMarkerPos().x > 1800.0f) {
		if (player_->GetPosition().x > 1800.0f) {
			if (audio_->IsPlaying(BGMHandle_)) {
				audio_->StopWave(BGMHandle_);
			}

			sceneNum = GAMESCENE;
		}
	}
}

void TitleScene::Draw() {
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

	// シーン遷移エリアの描画
	title2gameSceneAria_->Draw();

	// もしチュートリアルが始まってたら
	if (TitleTutrialStart) {
		enemyManager_->Draw();
	}
	// プレイヤーの描画処理
	player_->Draw();

	// ロゴの描画
	titleLogo_->Draw();
	
	// チュートリアルが始まったら描画
	if (TitleTutrialStart) {
		player_->DrawUI();
	}



	player_->DrawCursor();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion
}

void TitleScene::InitializeGrobalVariables() {
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "TitleScene";

	// 作ったグループにそれぞれアイテムを追加
	gloVars->CreateGroup(groupName);
	gloVars->AddItem(groupName, "enemyEndPos0_", enemyEndPos_[0]);
	gloVars->AddItem(groupName, "enemyEndPos1_", enemyEndPos_[1]);
	gloVars->AddItem(groupName, "enemyEndPos2_", enemyEndPos_[2]);
	gloVars->AddItem(groupName, "enemyEndPos3_", enemyEndPos_[3]);
	gloVars->AddItem(groupName, "enemyEndPos4_", enemyEndPos_[4]);
	gloVars->AddItem(groupName, "enemyEndPos5_", enemyEndPos_[5]);

}

void TitleScene::ApplyGrobalVariables() { 
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "TitleScene";
	// 作ったグループにあるアイテムから値を取得
	enemyEndPos_[0] = gloVars->GetVector2Value(groupName, "enemyEndPos0_");
	enemyEndPos_[1] = gloVars->GetVector2Value(groupName, "enemyEndPos1_");
	enemyEndPos_[2] = gloVars->GetVector2Value(groupName, "enemyEndPos2_");
	enemyEndPos_[3] = gloVars->GetVector2Value(groupName, "enemyEndPos3_");
	enemyEndPos_[4] = gloVars->GetVector2Value(groupName, "enemyEndPos4_");
	enemyEndPos_[5] = gloVars->GetVector2Value(groupName, "enemyEndPos5_");
}

void TitleScene::CheckAllCollision() { 
	// 変数の用意
	Vector2 targetA, targetB; 
	
	// プレイヤーの弾リストを取得
	const std::list<PlayerBullet*>& playerBullet = player_->GetBullets();
	// 敵
	const std::list<Enemy*>& enemys = enemyManager_->GetEnemyLists();
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
			if (!player_->GetIsInvisible() && !enemy->GetIsDead()) {
				// コールバック
				if (enemy->IsParasite()) {
					player_->AddTails();
					if (player_->GetTail() >= 6) {
						// gameTimer += pickUpTailTime;
					}

					enemy->SetIsDead(true);
				}
				if (!enemy->IsParasite()) {
					player_->OnCollision();
					//issceneShake = true;
					//sceneaAmplitNum = 40;
					// gameTimer -= eneBulletDamage;
					enemy->SetIsDead(true);
				}
				//
				// killCount_ += 1;

			}
			//
			else if (player_->GetIsInvisible()) {
				if (enemy->IsParasite()) {
					// gameTimer += pickUpTailTime;

					player_->AddTails();
					enemy->SetIsDead(true);
				}
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
			float bulletRadius = playerBullet_->GetRadius();

			if (distance <= radius) {
				// コールバック
				if (!enemy->IsParasite() && !enemy->GetIsDead()) {
					if (!playerBullet_->IsCollapse()) {
						enemy->OnCollision();
						//killCount_ += 1;
					}
					if (bulletRadius < 32.0f) {
						playerBullet_->OnCollision();
					}
				}
			}
		}
	}
#pragma endregion


}
