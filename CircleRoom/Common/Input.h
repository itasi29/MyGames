#pragma once
#include <array>
#include <map>
#include <unordered_map>
#include <string>
#include "Vec2.h"

enum class InputType
{
	keybd,	// キーボード
	pad		// パッド
};

using InputTable_t = std::unordered_map<std::string, std::map<InputType, int>>;

class KeyConfigScene;

class Input
{
	friend KeyConfigScene;
private:
	// コマンド名と入力をペアにしたテーブル
	// キーボードの時は keybd&KEY_INPUT_○○で、
	// パッドの時はpad&PAD_INPUT_○○
	// をチェックする
	InputTable_t m_commandTable;
	std::vector<std::string> m_exclusiveKeyConfigCommands;

	// コマンドの入力を覚えておく
	std::map<std::string, bool> m_inputDate;		// 現在の入力
	std::map<std::string, bool> m_lastInputDate;	// 直前の入力

	// スティック情報
	Vec2 m_inputStickDate;

	const InputTable_t GetCommandTable() const;

public:
	Input();
	/// <summary>
	/// 入力の情報を更新する
	/// </summary>
	void Update();
	/// <summary>
	/// 指定のコマンドが押された瞬間なのか
	/// </summary>
	/// <param name="command">コマンド文字列</param>
	/// <returns>true:押された瞬間 / false:押されていないか押しっぱ</returns>
	bool IsTriggered(const char* command) const;
	/// <summary>
	/// 指定のコマンドが押されているか
	/// </summary>
	/// <param name="command">コマンド文字列</param>
	/// <returns>true:押されている / false:押されていない</returns>
	bool IsPress(const char* command) const;
	/// <summary>
	/// 指定のコマンドが離された瞬間なのか
	/// </summary>
	/// <param name="command">コマンド文字列</param>
	/// <returns>true:離された瞬間 / false:それ以外</returns>
	bool IsReleased(const char* command) const;

	/// <summary>
	/// スティックの情報を渡す
	/// </summary>
	/// <returns>スティック情報</returns>
	Vec2 GetStickDate() const { return m_inputStickDate; }

	float GetStickTilt() const { return m_inputStickDate.Length(); }
};

