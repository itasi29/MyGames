#include "CsvLoader.h"
#include "StringUtility.h"
#include <cassert>
#include <fstream>
#include <sstream>

CsvOut_t CsvLoader::Load(const std::string& path, const CsvHeader_t& header)
{
	// �t�@�C���ǂݍ���
	std::wifstream ifs(path.c_str());
	if (!ifs)
	{
		assert(false);
		return CsvOut_t();
	}

	/* �ϐ� */
	std::wstring strBuf;	// CSV����1�s�ǂݍ��񂾕������ۑ�
	ConmaStr_t strConmaBuf;	// �ǂݍ��񂾕������','�ŕ�������Ԃŕۑ�
	CsvIndex_t indexData;		// �v�f���

	/* indexData�̍쐬 */
	std::getline(ifs, strBuf);
	// HACK: �Ȃ����m��Ȃ�����3�ɃS�~�����邽�ߏ���
	strBuf = strBuf.substr(3);
	strConmaBuf = StringUtility::Split(strBuf, ',');
	MakeIndex(header, strConmaBuf, indexData);

	/* �f�[�^�̓ǂݍ��� */
	CsvOut_t result;
	while (std::getline(ifs, strBuf))
	{
		strConmaBuf = StringUtility::Split(strBuf, ',');

		result.emplace_back(LoadInfo(header, strConmaBuf, indexData));
	}

    return result;
}


void CsvLoader::MakeIndex(const CsvHeader_t& header, const ConmaStr_t& strConmaBuf, CsvIndex_t& indexData)
{
	int index = 0;
	// ���ɏ�����Ă��镶��������ׂēǂݍ���
	for (auto& headerName : strConmaBuf)
	{
		// �n���ꂽHeader������
		for (auto& header : header)
		{
			// ���ɏ����ꂽ������Ɠn���ꂽHeader�̕����񂪓���
			if (headerName == header.first)
			{
				// �v�f�Ƃ��Ēǉ�
				indexData[headerName] = index;
				// �v�f�ԍ�������
				++index;
				// ���̓��̕������
				break;
			}
		}
	}
}

CsvData_t CsvLoader::LoadInfo(const CsvHeader_t& header, const ConmaStr_t& strConmaBuf, const CsvIndex_t& indexData)
{
	// Header�������ׂĉ�
	CsvData_t datas;
	for (const auto& info : header)
	{
		auto& data = datas[info.first];
		int index = indexData.at(info.first);

		// �^�ɍ��킹�ăf�[�^���擾
		if (info.second == Type::kInt)
		{
			data.tInt = std::stoi(strConmaBuf[index]);
		}
		else if (info.second == Type::kFloat)
		{
			data.tFloat = std::stof(strConmaBuf[index]);
		}
		else if (info.second == Type::kDouble)
		{
			data.tDouble = std::stod(strConmaBuf[index]);
		}
		else if (info.second == Type::kWstr)
		{
			data.tWstr = strConmaBuf[index];
		}
	}
	return datas;
}

