#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>
#include <memory>

// シーン
enum Scene { TITLE, GAMESCENE, CLEAR, };

/// <summary>
/// ベースシーンクラス
/// </summary>
class BaseScene {
public: // メンバ関数

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize();
	
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(){};
	
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(){};

	/// <summary>
	/// シーン番号の取得
	/// </summary>
	/// <returns>シーン番号</returns>
	int GetSceneNum() { return sceneNum; }

protected: // メンバ変数
	// dxクラス
	DirectXCommon* dxCommon_ = nullptr;
	
	// 入力クラス
	Input* input_ = nullptr;
	
	// 音クラス
	Audio* audio_ = nullptr;

	// 静的シーン番号
	static int sceneNum;
	
	// ボスを倒したかどうか
	static bool isBossDead;

};
