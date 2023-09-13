#include "ImGuiManager.h"
#include "TextureManager.h"
#include "ResultScene.h"
#include <cassert>
#include <cmath>

/// <summary>
/// コンストクラタ
/// </summary>
ResultScene::ResultScene() {}

/// <summary>
/// デストラクタ
/// </summary>
ResultScene::~ResultScene() {}

void ResultScene::Initialize() {
	sceneNum = CLEAR;
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	Scroll::GetInstance()->Initialize();

	player_.release();
	title2gameSceneAria_.release();
	title_.release();

	titleLogoPos_ = titleLogoStartPos_;
	ariaPos_ = ariaStartPos_;
	titleLogo_t_ = 0.0f;
	titleLogoMove_ = false;
	isSceneChange = false;
	ariaColor_ = {0.6f, 0.6f, 0.6f, 0.6f};
	aria_t_ = 0.0f;
	isAriaMove_ = false;

#pragma region 背景
	backTex = TextureManager::Load("white1x1.png");
	back.reset(Sprite::Create(backTex, {0.0f, 0.0f}, {0.01f, 0.01f, 0.01f, 1.0f}, {0.0f, 0.0f}));
	Vector2 size = {1920, 1080};
	back->SetSize(size);
#pragma endregion

#pragma region シーン遷移エリア
	ariaTex_ = TextureManager::Load("T2GaSceneAria.png");
	title2gameSceneAria_.reset(Sprite::Create(ariaTex_, ariaPos_, ariaColor_, {0.0f, 0.0f}));
	ariaPos_ = {1800.0f, 0.0f};

#pragma endregion

#pragma region タイトル
	titleLogoPos_ = {WinApp::kWindowWidth / 2, (WinApp::kWindowHeight / 2) / 2};
	titleLogoStartPos_ = {WinApp::kWindowWidth / 2, (WinApp::kWindowHeight / 2) / 2};
	titleLogoEndPos_ = {WinApp::kWindowWidth / 2, (WinApp::kWindowHeight / 2) / 2 - 10.0f};
	titleTex_ = TextureManager::Load("result.png");
	title_.reset(Sprite::Create(titleTex_, titleLogoPos_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	title_->SetTextureRect({0.0f, 0.0f}, {950.0f, 450.0f});
	title_->SetSize({950.0f, 540.0f});
#pragma endregion

#pragma region プレイヤー
	// プレイヤー生成
	player_ = std::make_unique<Player>();
	player_->SetIsGameStart(false);
	// プレイヤーの初期化処理
	player_->Initialize();
#pragma endregion 
	
	title2gameSceneAria_->SetPosition(ariaStartPos_);
}

void ResultScene::Update() {
	if (isBossDead) {
		title_->SetTextureRect({0.0f, 0.0f}, {950.0f, 540.0f});
	} else if (!isBossDead) {
		title_->SetTextureRect({950.0f, 0.0f}, {950.0f, 540.0f});

	}
	title_->SetPosition(titleLogoPos_);
	
	// プレイヤーの更新処理
	player_->Update();

	if (!isSceneChange) {
		NotTutorialUpdate();
	}
	//
	else if (isSceneChange) {

		TutorialUpdate();
	}

}

void ResultScene::NotTutorialUpdate() {
	if (!titleLogoMove_) {
		titleLogoPos_ = MyMath::EaseInCubicF(titleLogo_t_, titleLogoStartPos_, titleLogoEndPos_);
		MyMath::CountT(titleLogo_t_, 0.0f, titleLogoMove_, true, 0.1f);
	}
	if (titleLogoMove_) {
		titleLogoPos_ = MyMath::EaseInCubicF(titleLogo_t_, titleLogoEndPos_, titleLogoStartPos_);
		MyMath::CountT(titleLogo_t_, 0.0f, titleLogoMove_, false, 0.1f);
		if (!titleLogoMove_) {
			titleLogoPos_ =
			    MyMath::EaseInCubicF(titleLogo_t_, titleLogoStartPos_, titleLogoEndPos_);
			MyMath::CountT(titleLogo_t_, 0.0f, titleLogoMove_, true, 0.1f);
		}
	}
	if (input_->IsTriggerMouse(0)) {
		isSceneChange = true;
	}
}

void ResultScene::TutorialUpdate() {
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
		titleLogoPos_ =
		    MyMath::EaseInCubicF(titleLogo_t_, titleLogoEndPos_, {titleLogoEndPos_.x, -200.0f});
		MyMath::CountT(titleLogo_t_, 1.0f, titleLogoMove_, true, 0.1f);
	}

	if (player_->GetMarkerPos().x > 1800.0f) {
		if (player_->GetPosition().x > 1800.0f) {
			sceneNum = TITLE;
		}
	}
}

void ResultScene::Draw() {
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

	// プレイヤーの描画処理
	player_->Draw();

	//
	title_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion
}
