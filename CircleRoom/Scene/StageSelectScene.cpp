#include <DxLib.h>
#include <sstream>

#include "StringUtility.h"
#include "Vec2.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "Stage/StageManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/FontSystem.h"
#include "Input.h"

#include "Stage/Stage1_1.h"
#include "Stage/Stage1_2.h"
#include "Stage/Stage1_3.h"
#include "Stage/Stage1_4.h"
#include "Stage/Stage1_5.h"
#include "Stage/Stage1_6.h"
#include "Stage/Stage1_7.h"
#include "Stage/Stage1_8.h"
#include "Stage/Stage1_9.h"

#include "StageSelectScene.h"

namespace
{
	constexpr int kAppeaInterval = 5;
	constexpr int kMenuMargin = 120;

	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// ����������̐F
	constexpr unsigned int kYellowColor = 0xffde00;

	constexpr int kStageNum = 5;

	// �X�e�[�W�g�̑傫��
	constexpr int kStageSize = 64;
	// �t���[���̑���
	constexpr int kStageFrameThickness = 4;
	// �X�e�[�W�Ԃ̋�
	constexpr int kStageMargine = 128;

	// �X�^�[�g�ʒu
	constexpr int kStartX = 512;
	constexpr int kStartY = 256;

	// ��񕶎���`��ʒu
	constexpr int kDrawStringX = 200;
	constexpr int kDrawStringY = 216;

	// �X�e�[�W�̏c���̐�
	constexpr int kLineNum = 3;
	constexpr int kRowNum = 3;
	// �X�e�[�W����
	const std::string kStageStr[kLineNum][kRowNum] =
	{
		{"Stage1-5", "StageBoss", "Stage1-7"},
		{"Stage1-3", "Stage1-4", "Stage1-6"},
		{"Stage1-2", "Stage1-1", "Master"}
	};
}

StageSelectScene::StageSelectScene(GameManager& mgr, Input& input) :
	Scene(mgr),
	m_indexRow(0),
	m_indexLine(0),
	m_fadeFrame(0)
{
	CurrosrPos();

	m_stageData["Stage1-1"] = std::make_shared<Stage1_1>(mgr, input);
	m_stageData["Stage1-2"] = std::make_shared<Stage1_2>(mgr, input);
	m_stageData["Stage1-3"] = std::make_shared<Stage1_3>(mgr, input);
	m_stageData["Stage1-4"] = std::make_shared<Stage1_4>(mgr, input);
	m_stageData["Stage1-5"] = std::make_shared<Stage1_5>(mgr, input);
	m_stageData["Stage1-6"] = std::make_shared<Stage1_6>(mgr, input);
	m_stageData["Stage1-7"] = std::make_shared<Stage1_7>(mgr, input);
	m_stageData["StageBoss"] = std::make_shared<Stage1_8>(mgr, input);
	m_stageData["Master"] = std::make_shared<Stage1_9>(mgr, input);

	m_soundSys = mgr.GetSound();

	auto& file = mgr.GetFile();
	m_frame = file->LoadGraphic(L"UI/selectFrame.png");
	m_nowPos = file->LoadGraphic(L"UI/nowPos.png");
	m_lock = file->LoadGraphic(L"UI/lock.png");

	for (int x = 0; x < kRowNum; x++)
	{
		for (int y = 0; y < kLineNum; y++)
		{
			auto& name = kStageStr[y][x];
			std::wostringstream oss1;
			oss1 << L"UI/" << name.c_str() << L".png";
			m_stage[name][0] = file->LoadGraphic(oss1.str());

			std::wostringstream oss2;
			oss2 << L"UI/n" << name.c_str() << L".png";
			m_stage[name][1] = file->LoadGraphic(oss2.str());
		}
	}

	m_selectSe = file->LoadSound(L"Se/select.mp3");
}

StageSelectScene::~StageSelectScene()
{
}

void StageSelectScene::Update(Input& input)
{
	m_fadeFrame++;

	// �㉺�̃C���f�b�N�X�̕ύX
	if (input.IsTriggered("up"))
	{
		m_indexLine = (m_indexLine - 1 + kLineNum) % kLineNum;
		m_fadeFrame = 0;
	}
	if (input.IsTriggered("down"))
	{
		m_indexLine = (m_indexLine + 1) % kLineNum;
		m_fadeFrame = 0;
	}

	// ���E�̃C���f�b�N�X�̕ύX
	if (input.IsTriggered("left"))
	{
		m_indexRow = (m_indexRow - 1 + kRowNum) % kRowNum;
		m_fadeFrame = 0;
	}
	if (input.IsTriggered("right"))
	{
		m_indexRow = (m_indexRow + 1) % kRowNum;
		m_fadeFrame = 0;
	}

	if (input.IsTriggered("OK"))
	{
		// �X�e�[�W���Ȃ��Ȃ�I��
		if (kStageStr[m_indexLine][m_indexRow] == "None") return;
		auto stgName = kStageStr[m_indexLine][m_indexRow];

		// ���̃X�e�[�W���N���A����Ă��Ȃ���ΏI��
		if (!m_mgr.GetStage()->IsClearStage(stgName)) return;

		m_soundSys->PlaySe(m_selectSe->GetHandle());

		m_mgr.GetStage()->ChangeStage(m_stageData[stgName]);

		m_mgr.GetScene()->PopScene();
	}
}

void StageSelectScene::Draw()
{
	for(int x = 0; x < kRowNum; x++)
	{
		int drawX = static_cast<int>(kStageMargine * x) + kStartX;
		bool isIndexRow = (m_indexRow == x);

		for (int y = 0; y < kLineNum; y++)
		{
			int drawY = static_cast<int>(kStartY + kStageMargine * y);
			// ���ݑI�����Ă���X�e�[�W
			const std::string& stageName = kStageStr[y][x];
			// �t���[���̕`��
			if (isIndexRow && m_indexLine == y)
			{
				int frame = (m_fadeFrame % 80) - 40;
				float rate = fabsf(static_cast<float>(frame)) / 40.0f;
				int alpha = static_cast<int>(255 * rate);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
				DrawRotaGraph(drawX, drawY, 1.0, 0.0, m_frame->GetHandle(), true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


				// ���̕`��
				DrawInf(stageName);
			}

			// �X�e�[�W�̕`�悪�����̏ꍇ�͈ȉ��͕̂`�悵�Ȃ�
			if (kStageStr[y][x] == "None") continue;



			// �X�e�[�W�̕`��
			if (m_mgr.GetStage()->IsClearStage(stageName))
			{
				DrawRotaGraph(drawX, drawY, 1.0, 0.0, m_stage[stageName][0]->GetHandle(), true);
			}
			else
			{
				DrawRotaGraph(drawX, drawY, 1.0, 0.0, m_stage[stageName][1]->GetHandle(), true);
				DrawGraph(drawX + 16, drawY + 12, m_lock->GetHandle(), true);
			}

			// ���݂���X�e�[�W�̕`��
			if ((stageName) == m_mgr.GetStage()->GetStageName())
			{
				DrawRotaGraph(static_cast<int>(drawX), static_cast<int>(drawY),
					1.0, 0.0, m_nowPos->GetHandle(), true);

			}
		}
	}
}

void StageSelectScene::DrawInf(const std::string& str)
{
	int font = m_mgr.GetFont()->GetHandle(32);

	const auto& wstr = StringUtility::StringToWString(str);

	int y = kDrawStringY;

	// �X�e�[�W���̕`��
	DrawStringToHandle(kDrawStringX, y, wstr.c_str(), kWhiteColor, font);
	// �x�X�g�^�C���̕`��
	y += 96;
	DrawStringToHandle(kDrawStringX, y, L"�x�X�g�^�C��", kWhiteColor, font);
	y += 48;

	int bestTime = m_mgr.GetStage()->GetBestTime(str);
	int minSec = (bestTime * 1000 / 60) % 1000;
	int sec = (bestTime / 60) % 60;
	int min = bestTime / 3600;
	DrawFormatStringToHandle(kDrawStringX, y, kYellowColor, font, L"%02d:%02d.%03d", min, sec, minSec);

	y += 48;

	// �E���ꂽ��ނ̕`��
	m_stageData[str]->DrawKilledEnemyType(kDrawStringX, y);

}

void StageSelectScene::CurrosrPos()
{
	const auto& nowStage = m_mgr.GetNowStage();

	for (int x = 0; x < kRowNum; x++)
	{
		for (int y = 0; y < kLineNum; y++)
		{
			if (nowStage == kStageStr[y][x])
			{
				m_indexRow = x;
				m_indexLine = y;

				return;
			}
		}
	}
}
