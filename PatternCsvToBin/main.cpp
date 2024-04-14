#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

namespace
{
	const char* const kInPath = "PatternMaster.csv";
	const char* const kOutPath = "PatternMaster.bin";

	enum Index : int {
		kNo,
		kCreateNum,
		kCreateFrame,
		kSpeedMul,
		kCreatePos
	};

	struct CreateData {
		int createFrame = 0;
		float speedMul = 0.0f;
		float createPos = 0.0f;
	};
	struct PatternData {
		int createNum = 0;
		std::vector<CreateData> data;
	};

	constexpr float kVar = 1.0f;
	struct Header
	{
		float var = kVar;
		int size = 0;
	};
}

using Data_t = std::unordered_map<int, PatternData>;

void LoadCsv(Data_t& data);
void WriteBin(Data_t& data);
std::vector<std::wstring> Split(const std::wstring& str, const char del);

int main()
{
	Data_t data;
	LoadCsv(data);
	WriteBin(data);

	return 0;
}

void LoadCsv(Data_t& data)
{
	// �t�@�C���ǂݍ���
	std::wifstream ifs(kInPath);
	if (!ifs) {
		assert(false);
		return;
	}

	std::wstring strBuf;
	std::vector<std::wstring> strConmaBuf;

	// �]���ȕ����̓ǂݍ���
	std::getline(ifs, strBuf);

	// �f�[�^�m�F�p
	int count;
	bool isNextGet = true;
	// �f�[�^�ۑ��p
	int no;
	int createNum;

	// �f�[�^���ׂēǂݍ���
	while (std::getline(ifs, strBuf)) {
		strConmaBuf = Split(strBuf, ',');

		if (isNextGet) {
			// �ԍ��擾
			no = std::stoi(strConmaBuf[kNo]);

			// �������擾
			createNum = std::stoi(strConmaBuf[kCreateNum]);
			data[no].createNum = createNum;

			// �J�E���g������
			count = 0;
			// ���f�[�^���Ƃ�Ȃ��悤��
			isNextGet = false;
		}

		// �e��p�����[�^�擾
		CreateData temp;
		temp.createFrame = std::stoi(strConmaBuf[kCreateFrame]);
		temp.speedMul = std::stof(strConmaBuf[kSpeedMul]);
		temp.createPos = std::stof(strConmaBuf[kCreatePos]);

		// �p�����[�^���
		data[no].data.push_back(temp);

		// ���f�[�^�ڍs�m�F
		++count;
		if (count >= createNum) {
			isNextGet = true;
		}
	}

	return;
}

void WriteBin(Data_t& data)
{
	FILE* fp;

	// �t�@�C���ǂݍ���
	auto err = fopen_s(&fp, kOutPath, "wb");
	if (err != 0) {
		assert(false);
		return;
	}

	Header header;
	header.var = kVar;
	header.size = static_cast<int>(data.size());
	fwrite(&header, sizeof(header), 1, fp);

	for (auto& ref : data) {
		// �ԍ��̕ۑ�
		auto no = ref.first;
		fwrite(&no, sizeof(no), 1, fp);

		auto pattern = ref.second;
		// �������̕ۑ�
		fwrite(&pattern.createNum, sizeof(pattern.createNum), 1, fp);

		// �����f�[�^�̕ۑ�
		fwrite(pattern.data.data(), sizeof(CreateData) * pattern.createNum, 1, fp);
	}

	fclose(fp);

	return;
}

std::vector<std::wstring> Split(const std::wstring& str, const char del)
{
	// ��؂�J�n�ʒu
	int first = 0;
	// ��؂�ŏI�ʒu
	int last = static_cast<int>(str.find_first_of(del));

	std::vector<std::wstring> result;

	while (first < str.size()) {
		// ��؂������̂����ʂɓ���Ă���
		std::wstring subStr(str, first, last - first);

		result.push_back(subStr);

		// �ʒu�X�V
		first = last + 1;
		last = static_cast<int>(str.find_first_of(del, first));
		if (last == std::wstring::npos) {
			last = static_cast<int>(str.size());
		}
	}

	return result;
}