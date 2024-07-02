#pragma once
#include <unordered_map>
#include <string>

enum InputType
{
	Key,
	Pad
};

using InputTable_t = std::unordered_map<std::string, std::unordered_map<InputType, int>>;
using InputData_t = std::unordered_map<int, std::unordered_map<std::string, bool>>;

class Input
{
private :
	Input();

	Input(const Input& input) = delete;
	void operator=(const Input& input) = delete;

public:
	static Input& GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();
	/// <summary>
	/// 入力情報の更新
	/// </summary>
	/// <param name="no">タイプ番号</param>
	void Update(int no);

	/// <summary>
	/// 押されているか
	/// </summary>
	/// <param name="no">タイプ番号</param>
	/// <param name="command">コマンド</param>
	/// <returns>true : 押している / false : 押していない</returns>
	bool IsPress(int no, const char* const command) const;
	/// <summary>
	/// 押した瞬間
	/// </summary>
	/// <param name="no">タイプ番号</param>
	/// <returns>true : 押した瞬間 / false : 押していないor押し続けている</returns>
	bool IsTriggerd(int no, const char* const command) const;
	/// <summary>
	/// 離した瞬間
	/// </summary>
	/// <param name="no">タイプ番号</param>
	/// <returns>true : 離した / false : 押していないor押し続けている</returns>
	bool IsReleased(int no, const char* const command) const;

private:
	int GetInputType(int no);

private:
	// コマンドと入力のペア
	InputTable_t m_commandTable;

	// 入力の記憶
	InputData_t m_isInput;		// 現在の入力
	InputData_t m_isLastInput;	// 直前の入力
};

