#pragma once
#include <vector>
#include <string>
#include "Vec2.h"

enum class MapDir
{
	kUp,
	kDown,
	kRight,
	kLeft,
	kMax
};

struct LinkInfo
{
	bool isUp = false;
	std::string upName = {};
	bool isDown = false;
	std::string downName = {};
	bool isRight = false;
	std::string rightName = {};
	bool isLeft = false;
	std::string leftName = {};
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

	/// <summary>
	/// �ڑ��m�F
	/// </summary>
	/// <param name="stage">���݃X�e�[�W��</param>
	/// <param name="dir">����</param>
	/// <returns>true: �ڑ����Ă��� / false:�ڑ����Ă��Ȃ�</returns>
	bool IsConect(const std::string& stage, MapDir dir) const;
	/// <summary>
	/// �ڑ����Ă���Ƃ���̖��O���擾
	/// </summary>
	/// <param name="stage">���݃X�e�[�W��</param>
	/// <param name="dir">����</param>
	/// <returns>�X�e�[�W��</returns>
	std::string GetConectName(const std::string& stage, MapDir dir) const;

	// �X�e�[�W�ʒu��Ԃ�
	Vec2 GetPos(const std::string& stage) const;

private:
	// �v�f�ԍ��擾
	int GetIndex(const std::string& stage) const;

	void Load();

	// �����ʏ��m�F
	void CheckUp(MapInfo& data, bool isStart, int index, const std::vector<std::string>& strInfo, const std::vector<std::string>& strName);
	void CheckDown(MapInfo& data, bool isLoop, int index, const std::vector<std::string>& strInfo, const std::vector<std::string>& strName);
	void CheckLeft(MapInfo& data, int index, const std::vector<std::string>& str);
	void CheckRight(MapInfo& data, int index, const std::vector<std::string>& str);

private:
	std::vector<MapInfo> m_data;
	int m_line;
	int m_row;
};

