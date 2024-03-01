#pragma once]
#include <vector>
#include <string>

struct LinkInfo
{
	bool isUp = false;
	bool isDown = false;
	bool isRight = false;
	bool isLeft = false;
};

struct MapInfo
{
	std::string name;
	LinkInfo info;
};

class StageMap
{
public:
	StageMap();

private:
	void Load();

	void CheckUp(int index, int row, int line, const std::vector<std::string>& str);
	void CheckDown(int index, int row, int line, const std::vector<std::string>& str);
	void CheckLeft(int index, int row, int line, const std::vector<std::string>& str);
	void CheckRight(int index, int row, int line, const std::vector<std::string>& str);

private:
	std::vector<MapInfo> m_data;
	int m_line;
	int m_row;
};

