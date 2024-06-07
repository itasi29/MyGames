#pragma once
#include <unordered_map>
#include <string>
#include <array>
#include <vector>
#include "Game.h"

enum class InputType
{
	Key,
	Pad,
};
using CommandTable_t = std::unordered_map<std::string, std::unordered_map<InputType, int>>;
using InputData_t = std::unordered_map<std::string, bool>;
using InputLog_t = std::array<InputData_t, Game::kMaxPadNum>;

struct Input final
{
private:
	Input();
	~Input();

	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

public:
	static Input& GetInstance();

	void Update();

	bool IsTrigger(const std::string& cmd, int no = 0) const;
	bool IsPress(const std::string& cmd, int no = 0) const;
	bool IsRelease(const std::string& cmd, int no = 0) const;

private:
	// ÉçÉOÇÃêî
	static constexpr int kLogNum = 16;

private:
	std::array<InputLog_t, kLogNum> m_log;
	CommandTable_t m_commandTable;
};

