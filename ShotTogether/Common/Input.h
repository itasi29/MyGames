#pragma once
#include <array>
#include <map>
#include <unordered_map>
#include <string>

enum class InputType
{
	keybd,	// キーボード
	pad		// パッド
};

using InputTable_t = std::unordered_map<std::string, std::map<InputType, std::vector<int>>>;

class KeyConfigScene;
class PadConfigScene;

class Input
{
	friend KeyConfigScene;
	friend PadConfigScene;
private:
	// コマンド名と入力をペアにしたテーブル
	InputTable_t m_commandTable;

	// コマンドの入力を覚えておく
	std::map<std::string, bool> m_inputDate;		// 現在の入力
	std::map<std::string, bool> m_lastInputDate;	// 直前の入力

	// 何かしらのボタン押された情報
	bool m_isAnyPush;
	bool m_isLastAnyPush;

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
	/// 何かしらのボタンが押されたか
	/// </summary>
	/// <returns>true:押された瞬間 / false:押されていないか押しっぱ</returns>
	bool IsAnyTriggerd();
	/// <summary>
	/// 指定のコマンドが押されているか
	/// </summary>
	/// <param name="command">コマンド文字列</param>
	/// <returns>true:押されている / false:押されていない</returns>
	bool IsPress(const char* command) const;
	/// <summary>
	/// 何かしらのボタンが押されているか
	/// </summary>
	/// <returns>true:押されている / false:押されていない</returns>
	bool IsAnyPress() const;
	/// <summary>
	/// 指定のコマンドが押されていないか
	/// </summary>
	/// <param name="command">コマンド文字列</param>
	/// <returns>true:押されていない / false:押されている</returns>
	bool IsNotPress(const char* command) const;
	/// <summary>
	/// 指定のコマンドが離された瞬間なのか
	/// </summary>
	/// <param name="command">コマンド文字列</param>
	/// <returns>true:離された瞬間 / false:それ以外</returns>
	bool IsReleased(const char* command) const;
	/// <summary>
	/// 押された瞬間＋押されて行って時間が経過されている
	/// </summary>
	/// <param name="command">コマンド文字列</param>
	/// <param name="frame">フレームの参照</param>
	/// <param name="frameInterval">フレームの間隔</param>
	/// <returns></returns>
	bool IsReepat(const char* command, int& frame, int frameInterval) const;
};

