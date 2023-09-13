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
TitleScene::~TitleScene() {
	if (audio_->IsPlaying(BGMHandle_)) {
		audio_->StopWave(BGMHandle_);
	}
	if (audio_->IsPlaying(pickUpTailSEHandle_)) {
		audio_->StopWave(pickUpTailSEHandle_);
	}
}

void TitleScene::Initialize() {
	// このシーンはタイトル
	sceneNum = TITLE;
	// 静的変数の初期化
	StaticValueInitialize();
	BGMHandle_ = Audio::GetInstance()->LoadWave("music/GameScene.wav");
	pickUpTailSEHandle_ = Audio::GetInstance()->LoadWave("music/pickUpTail.wav");

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
	backTex = TextureManager::Load("back.png");
	back.reset(Sprite::Create(backTex, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}));
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

#pragma region タイトル
	// タイトルロゴの位置
	leftClick_pos_ = {WinApp::kWindowWidth / 2, (WinApp::kWindowHeight / 2) / 2};

	// 線形補完用スタート位置
	leftClick_StartPos_ = {WinApp::kWindowWidth / 2, (WinApp::kWindowHeight / 2) / 2};

	// 線形補完用終わり位置
	leftClick_EndPos_ = {WinApp::kWindowWidth / 2, (WinApp::kWindowHeight / 2) / 2 - 10.0f};

	// タイトルロゴのテクスチャ読み込み
	leftClick_tex_ = TextureManager::Load("hidari.png");

	// タイトルロゴのスプライト生成
	leftClick_.reset(
	    Sprite::Create(leftClick_tex_, leftClick_pos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

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

	for (int i = 0; i < enemyNum; i++) {
		enemyManager_->AddEnemy(enemyEndPos_[i], {0.0f, 0.0f});
	}

#pragma region あてろ
	hitTex_ = TextureManager::Load("hitandpick.png");

	for (int i = 0; i < enemyNum; i++) {
		HitPos_[i] = {enemyEndPos_[i].x, enemyEndPos_[i].y - 20.0f};
		HitText_[i].reset(
		    Sprite::Create(hitTex_, HitPos_[i], {0.0f, 1.0f, 0.0f, 0.3f}, {0.5f, 0.5f}));
		HitText_[i]->SetTextureRect({0.0f, 0.0f}, {308.0f, 107.0f});
		HitText_[i]->SetSize({308.0f * 0.75f, 107.0f * 0.75f});
	}
#pragma endregion

#pragma region ひろえ
	pickedTex_ = TextureManager::Load("hitandpick.png");

	for (int i = 0; i < enemyNum; i++) {
		PickedPos_[i] = {enemyEndPos_[i].x, enemyEndPos_[i].y - 20.0f};
		PickedText_[i].reset(
		    Sprite::Create(pickedTex_, PickedPos_[i], {1.0f, 0.0f, 0.0f, 0.3f}, {0.5f, 0.5f}));
		PickedText_[i]->SetTextureRect({308.0f, 0.0f}, {308.0f, 107.0f});
		PickedText_[i]->SetSize({308.0f * 0.75f, 107.0f * 0.75f});
	}
#pragma endregion
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
	leftClick_->SetPosition(leftClick_pos_);

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
		leftClick_pos_ = MyMath::EaseInCubicF(titleLogo_t_, leftClick_StartPos_, leftClick_EndPos_);
		MyMath::CountT(titleLogo_t_, 0.0f, titleLogoMove_, true, 0.1f);
	}
	if (titleLogoMove_) {
		titleLogo_pos_ = MyMath::EaseInCubicF(titleLogo_t_, titleLogoEndPos_, titleLogoStartPos_);
		leftClick_pos_ = MyMath::EaseInCubicF(titleLogo_t_, leftClick_EndPos_, leftClick_StartPos_);
		MyMath::CountT(titleLogo_t_, 0.0f, titleLogoMove_, false, 0.1f);
		if (!titleLogoMove_) {
			titleLogo_pos_ =
			    MyMath::EaseInCubicF(titleLogo_t_, titleLogoStartPos_, titleLogoEndPos_);
			leftClick_pos_ =
			    MyMath::EaseInCubicF(titleLogo_t_, leftClick_StartPos_, leftClick_EndPos_);
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
		leftClick_pos_ =
		    MyMath::EaseInCubicF(titleLogo_t_, leftClick_EndPos_, {leftClick_EndPos_.x, -200.0f});
		MyMath::CountT(titleLogo_t_, 1.0f, titleLogoMove_, true, 0.1f);
	}

	enemyManager_->SetPlayer(player_->GetPosition());
	enemyManager_->SetTailSize(player_->GetTail());
	enemyManager_->EnemyUpdate();

	if (player_->GetMarkerPos().x > 1800.0f) {
		if (player_->GetPosition().x > 1800.0f) {
			player_->AudioStop();
			player_->AudioStop();

			if (audio_->IsPlaying(BGMHandle_)) {
				audio_->StopWave(BGMHandle_);
			}
			if (audio_->IsPlaying(BGMHandle_)) {
				audio_->StopWave(BGMHandle_);
			}

			sceneNum = GAMESCENE;
		}
	}
	{
		int i = 0;
		for (Enemy* enemy : enemyManager_->GetEnemyLists()) {
			//enemy->SetPosition(enemyEndPos_[i]);
			enemyEndPos_[i] = enemy->GetPosition();
			HitPos_[i] = {enemyEndPos_[i].x, enemyEndPos_[i].y - 128.0f};
			PickedPos_[i] = {enemyEndPos_[i].x, enemyEndPos_[i].y - 128.0f};
			HitText_[i]->SetPosition(HitPos_[i]);
			PickedText_[i]->SetPosition(PickedPos_[i]);
			i++;

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
		{
			int i = 0;
			for (Enemy* enemy : enemyManager_->GetEnemyLists()) {
				if (enemy->IsParasite()) {
					PickedText_[i]->Draw();
				} else if (!enemy->IsParasite()) {
					HitText_[i]->Draw();
				}
				i++;
			}
		}
	}
	// プレイヤーの描画処理
	player_->Draw();
	

	// ロゴの描画
	titleLogo_->Draw();
	leftClick_->Draw();
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

	gloVars->AddItem(groupName, "titleLogoStartPos_", titleLogoStartPos_);
	gloVars->AddItem(groupName, "titleLogoEndPos_", titleLogoEndPos_);

	gloVars->AddItem(groupName, "leftClick_StartPos_", leftClick_StartPos_);
	gloVars->AddItem(groupName, "leftClick_EndPos_", leftClick_EndPos_);
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

	titleLogoStartPos_ = gloVars->GetVector2Value(groupName, "titleLogoStartPos_");
	titleLogoEndPos_ = gloVars->GetVector2Value(groupName, "titleLogoEndPos_");
	
	leftClick_StartPos_ = gloVars->GetVector2Value(groupName, "leftClick_StartPos_");
	leftClick_EndPos_ = gloVars->GetVector2Value(groupName, "leftClick_EndPos_");
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
					Audio::GetInstance()->PlayWave(pickUpTailSEHandle_, false, SEvolume);

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
					Audio::GetInstance()->PlayWave(pickUpTailSEHandle_, false, SEvolume);

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
