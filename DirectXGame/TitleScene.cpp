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

	// いろいろ
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();




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

#pragma region 敵
	enemyTex_ = TextureManager::Load("Enemy_ver2.png");
	parasiteTex_ = TextureManager::Load("Parasite.png");
	particleTex_ = TextureManager::Load("Particle.png");
	collapseTex_ = TextureManager::Load("greenTailbreak.png");
	// 6体生成
	for (int i = 0; i < 6; i++) {
		enemyPos_[i] = {-300.0f, -300.0f};
		enemy_[i] = std::make_unique<Enemy>();
		enemy_[i]->SetTexture(enemyTex_);
		enemy_[i]->SetParasiteTexture(parasiteTex_);
		enemy_[i]->SetParticleTex(particleTex_);
		enemy_[i]->SetCollapseTexture(collapseTex_);

		enemy_[i]->SetPosition(enemyPos_[i]);
		enemy_[i]->Initialize();
	}
#pragma endregion
	title2gameSceneAria_->SetPosition(ariaStartPos_);

	// グローバル変数の初期化処理
	InitializeGrobalVariables();
}

void TitleScene::StaticValueInitialize() {
	resultTime = 0;
	resultScore = 0;
	resultKillCount = 0;
	isBossDead = false;
	is6Tails = false;
}

void TitleScene::Update() { 

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

	for (int i = 0; i < 6; i++) {
		if (!eneMove_) {
			enemyPos_[i] = MyMath::EaseInCubicF(eneMove_t_, enemyStartPos_[i], enemyEndPos_[i]);
			MyMath::CountT(eneMove_t_, 0.0f, eneMove_, true, 0.01f);
		}
		if (eneMove_) {
			if (rootEneMove) {
				enemyPos_[i] =
				    MyMath::EaseInCubicF(eneMove_t_, enemyEndPos_[i], enemyRootMOvePOs[i]);
				MyMath::CountT(eneMove_t_, 0.0f, rootEneMove, false, 0.01f);

			} else if (!rootEneMove) {
				enemyPos_[i] =
				    MyMath::EaseInCubicF(eneMove_t_, enemyRootMOvePOs[i], enemyEndPos_[i]);
				MyMath::CountT(eneMove_t_, 0.0f, rootEneMove, true, 0.01f);
			}
		}
		if (!enemy_[i]->GetIsDead()) {
			enemy_[i]->Update();
			enemy_[i]->SetPosition(enemyPos_[i]);
		}

	}

	if (player_->GetMarkerPos().x > 1800.0f) {
		if (player_->GetPosition().x > 1800.0f) {
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
		for (int i = 0; i < 6; i++) {
			// もし敵が死んでいなかったら
			if (!enemy_[i]->GetIsDead()) {
				// 敵の描画
				enemy_[i]->Draw();
			}
		}
	}
	// プレイヤーの描画処理
	player_->Draw();

	// ロゴの描画
	titleLogo_->Draw();
	
	// チュートリアルが始まったら描画
	if (TitleTutrialStart) {
		player_->DrawUI();
	}




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
	gloVars->AddItem(groupName, "enemyStartPos0_", enemyStartPos_[0]);
	gloVars->AddItem(groupName, "enemyStartPos1_", enemyStartPos_[1]);
	gloVars->AddItem(groupName, "enemyStartPos2_", enemyStartPos_[2]);
	gloVars->AddItem(groupName, "enemyStartPos3_", enemyStartPos_[3]);
	gloVars->AddItem(groupName, "enemyStartPos4_", enemyStartPos_[4]);
	gloVars->AddItem(groupName, "enemyStartPos5_", enemyStartPos_[5]);

	gloVars->AddItem(groupName, "enemyEndPos0_", enemyEndPos_[0]);
	gloVars->AddItem(groupName, "enemyEndPos1_", enemyEndPos_[1]);
	gloVars->AddItem(groupName, "enemyEndPos2_", enemyEndPos_[2]);
	gloVars->AddItem(groupName, "enemyEndPos3_", enemyEndPos_[3]);
	gloVars->AddItem(groupName, "enemyEndPos4_", enemyEndPos_[4]);
	gloVars->AddItem(groupName, "enemyEndPos5_", enemyEndPos_[5]);

	gloVars->AddItem(groupName, "enemyRootMOvePOs0", enemyRootMOvePOs[0]);
	gloVars->AddItem(groupName, "enemyRootMOvePOs1", enemyRootMOvePOs[1]);
	gloVars->AddItem(groupName, "enemyRootMOvePOs2", enemyRootMOvePOs[2]);
	gloVars->AddItem(groupName, "enemyRootMOvePOs3", enemyRootMOvePOs[3]);
	gloVars->AddItem(groupName, "enemyRootMOvePOs4", enemyRootMOvePOs[4]);
	gloVars->AddItem(groupName, "enemyRootMOvePOs5", enemyRootMOvePOs[5]);
}

void TitleScene::ApplyGrobalVariables() { 
	// グローバル変数系のシングルトンインスタンスを取得
	GlobalVariables* gloVars = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "TitleScene";
	// 作ったグループにあるアイテムから値を取得
	enemyStartPos_[0] = gloVars->GetVector2Value(groupName, "enemyStartPos0_");
	enemyStartPos_[1] = gloVars->GetVector2Value(groupName, "enemyStartPos1_");
	enemyStartPos_[2] = gloVars->GetVector2Value(groupName, "enemyStartPos2_");
	enemyStartPos_[3] = gloVars->GetVector2Value(groupName, "enemyStartPos3_");
	enemyStartPos_[4] = gloVars->GetVector2Value(groupName, "enemyStartPos4_");
	enemyStartPos_[5] = gloVars->GetVector2Value(groupName, "enemyStartPos5_");

	enemyEndPos_[0] = gloVars->GetVector2Value(groupName, "enemyEndPos0_");
	enemyEndPos_[1] = gloVars->GetVector2Value(groupName, "enemyEndPos1_");
	enemyEndPos_[2] = gloVars->GetVector2Value(groupName, "enemyEndPos2_");
	enemyEndPos_[3] = gloVars->GetVector2Value(groupName, "enemyEndPos3_");
	enemyEndPos_[4] = gloVars->GetVector2Value(groupName, "enemyEndPos4_");
	enemyEndPos_[5] = gloVars->GetVector2Value(groupName, "enemyEndPos5_");

	enemyRootMOvePOs[0] = gloVars->GetVector2Value(groupName, "enemyRootMOvePOs0");
	enemyRootMOvePOs[1] = gloVars->GetVector2Value(groupName, "enemyRootMOvePOs1");
	enemyRootMOvePOs[2] = gloVars->GetVector2Value(groupName, "enemyRootMOvePOs2");
	enemyRootMOvePOs[3] = gloVars->GetVector2Value(groupName, "enemyRootMOvePOs3");
	enemyRootMOvePOs[4] = gloVars->GetVector2Value(groupName, "enemyRootMOvePOs4");
	enemyRootMOvePOs[5] = gloVars->GetVector2Value(groupName, "enemyRootMOvePOs5");
}

void TitleScene::CheckAllCollision() { 
	// 変数の用意
	Vector2 targetA, targetB; 
	
	// プレイヤーの弾リストを取得
	const std::list<PlayerBullet*>& playerBullet = player_->GetBullets();
	// プレイヤーの位置取得
	targetA = player_->GetPosition();
#pragma region 敵とプレイヤー

	for (int i = 0; i < 6; i++) {
		// エネミーの位置取得
		targetB = enemy_[i]->GetPosition();
		float distance =
		    std::sqrtf(std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));
		float radius = player_->GetRadius() + enemy_[i]->GetRadius();
		// 交差判定
		if (distance <= radius) {
			if (!player_->GetIsInvisible() && !enemy_[i]->GetIsDead()) {
				// コールバック
				if (enemy_[i]->IsParasite()) {
					player_->AddTails();
					if (player_->GetTail() >= 6) {
						// gameTimer += pickUpTailTime;
					}

					enemy_[i]->SetIsDead(true);
				}
				if (!enemy_[i]->IsParasite()) {
					player_->OnCollision();
					//issceneShake = true;
					//sceneaAmplitNum = 40;
					// gameTimer -= eneBulletDamage;
					enemy_[i]->SetIsDead(true);
				}
				//
				// killCount_ += 1;

			}
			//
			else if (player_->GetIsInvisible()) {
				if (enemy_[i]->IsParasite()) {
					// gameTimer += pickUpTailTime;

					player_->AddTails();
					enemy_[i]->SetIsDead(true);
				}
			}
		}

	}
#pragma endregion
#pragma region プレイヤーの弾と敵
	// プレイヤーの弾と敵の衝突判定
	for (PlayerBullet* playerBullet_ : playerBullet) {
		targetA = playerBullet_->GetPosition();
		for (int i = 0; i < 6; i++) {
			targetB = enemy_[i]->GetPosition();
			float distance = std::sqrtf(
			    std::powf(targetA.x - targetB.x, 2) + std::powf(targetA.y - targetB.y, 2));
			float radius = playerBullet_->GetRadius() + enemy_[i]->GetRadius();
			float bulletRadius = playerBullet_->GetRadius();

			if (distance <= radius) {
				// コールバック
				if (!enemy_[i]->IsParasite() && !enemy_[i]->GetIsDead()) {
					if (!playerBullet_->IsCollapse()) {
						enemy_[i]->OnCollision();
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
