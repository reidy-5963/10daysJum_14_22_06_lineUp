#pragma once
#include <variant>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <Vector2.h>
#include <Vector3.h>
#include <json.hpp>
#include <json_fwd.hpp>

/// <summary>
/// グローバル変数クラス
/// </summary>
class GlobalVariables {
public: // サブ
	// int 
	// float 
	// Vector2
	// Vector3
	// 項目(上記の型を入れられる)
	using Item = std::variant<int32_t, float, Vector2, Vector3>;

	// グループ
	using Group = std::map<std::string, Item>;

public: // 静的メンバ関数
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static GlobalVariables* GetInstance();

public: // メンバ関数
	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	// 値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);

	// 値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);

	// 値のセット(Vector2)
	void SetValue(const std::string& groupName, const std::string& key, Vector2 value);

	// 値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, Vector3 value);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName">グループ</param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName">グループ</param>
	void LoadFile(const std::string& groupName);

	// 項目の追加(int)
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);

	// 項目の追加(float)
	void AddItem(const std::string& groupName, const std::string& key, float value);

	// 項目の追加(Vector3)
	void AddItem(const std::string& groupName, const std::string& key, Vector3 value);

	// 項目の追加(Vector2)
	void AddItem(const std::string& groupName, const std::string& key, Vector2 value);

	// 値の取得(int)
	int32_t GetIntValue(const std::string& groupName, const std::string& key);

	// 値の取得(float)
	float GetFloatValue(const std::string& groupName, const std::string& key);

	// 値の取得(Vector3)
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key);

	// 値の取得(Vector2)
	Vector2 GetVector2Value(const std::string& groupName, const std::string& key);

private: // メンバ関数
	// コピー禁止
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	const GlobalVariables& operator=(const GlobalVariables&) = delete;

private: // メンバ変数
	// 全データ
	std::map<std::string, Group> datas_;

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/GlobalVariables/";
};