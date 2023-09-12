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
	sceneNum =2;
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

#pragma region 背景
	backTex = TextureManager::Load("white1x1.png");
	back.reset(Sprite::Create(backTex, {0.0f, 0.0f}, {0.01f, 0.01f, 0.01f, 1.0f}, {0.0f, 0.0f}));
	Vector2 size = {1920, 1080};
	back->SetSize(size);
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

}

void ResultScene::Update() {
	if (isBossDead) {
		title_->SetTextureRect({0.0f, 0.0f}, {950.0f, 540.0f});
	} else if (!isBossDead) {
		title_->SetTextureRect({950.0f, 0.0f}, {950.0f, 540.0f});

	}

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

	title_->SetPosition(titleLogoPos_);
	// プレイヤーの更新処理
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

	// プレイヤーの描画処理

	//
	title_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion
}
