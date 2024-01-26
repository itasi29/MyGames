#include <DxLib.h>
#include <string>

#include "Vec2.h"
#include "GameManager.h"
#include "Stage/StageManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/FileBase.h"
#include "Input.h"

#include "Stage/Stage1_1.h"
#include "Stage/Stage1_2.h"
#include "Stage/Stage1_3.h"
#include "Stage/Stage1_4.h"
#include "Stage/Stage1_5.h"

#include "StageSelectScene.h"

namespace
{
	constexpr int kAppeaInterval = 5;
	constexpr int kMenuMargin = 120;

	constexpr int kStageNum = 5;

	// �X�e�[�W�g�̑傫��
	constexpr int kStageSize = 64;
	// �X�e�[�W�g�ƃt���[���̍�
	// MEMO:��ŉ摜�ɕύX���邽�߂����炭����
	constexpr int kStageFrameSize = 14;
	// �t���[���̑���
	constexpr int kStageFrameThickness = 4;
	// �X�e�[�W�Ԃ̋�
	constexpr int kStageMargine = 128;

	// �X�^�[�g�ʒu
	constexpr int kStartX = 400;
	constexpr int kStartY = 216;

	// �X�e�[�W�̏c���̐�
	constexpr int kLineNum = 3;
	constexpr int kRowNum = 2;
	// �X�e�[�W����
	const std::string kStageStr[kLineNum][kRowNum] = {
		{"1-5", "None"},
		{"1-4", "1-3"},
		{"1-2", "1-1"}
	};
}

StageSelectScene::StageSelectScene(GameManager& mgr) :
	Scene(mgr),
	m_indexRow(0),
	m_indexLine(0),
	m_fadeFrame(0)
{
	m_soundSys = mgr.GetSound();

	auto& file = mgr.GetFile();
	m_frame = file->LoadGraphic(L"UI/selectFrame.png");
	m_nowPos = file->LoadGraphic(L"UI/nowPos.png");
	m_lock = file->LoadGraphic(L"UI/lock.png");

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
		auto stgName = "Stage" + kStageStr[m_indexLine][m_indexRow];

		// ���̃X�e�[�W���N���A����Ă��Ȃ���ΏI��
		if (!m_mgr.GetStage()->IsClearStage(stgName)) return;

		m_soundSys->PlaySe(m_selectSe->GetHandle());

		if (stgName == "Stage1-1")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_1>(m_mgr, input));
		}
		if (stgName == "Stage1-2")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_2>(m_mgr, input));
		}
		if (stgName == "Stage1-3")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_3>(m_mgr, input));
		}
		if (stgName == "Stage1-4")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_4>(m_mgr, input));
		}
		if (stgName == "Stage1-5")
		{
			m_mgr.GetStage()->ChangeStage(std::make_shared<Stage1_5>(m_mgr, input));
		}
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
			// �t���[���̕`��
			if (isIndexRow && m_indexLine == y)
			{
				int frame = (m_fadeFrame % 80) - 40;
				float rate = fabsf(static_cast<float>(frame)) / 40.0f;
				int alpha = static_cast<int>(255 * rate);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
				DrawGraph(drawX - kStageFrameSize, kStartY + kStageMargine * y - kStageFrameSize, m_frame->GetHandle(), true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			// �X�e�[�W�̕`�悪�����̏ꍇ�͈ȉ��͕̂`�悵�Ȃ�
			if (kStageStr[y][x] == "None") continue;

			int drawY = static_cast<int>(kStartY + kStageMargine * y);

			// ���ݑI�����Ă���X�e�[�W
			// FIXME:���Ƃ��ĉ~��`�悵�Ă���̂ŁA�O���t�B�b�N���o������ύX
			std::string stageName = "Stage" + kStageStr[y][x];
			if ((stageName) == m_mgr.GetStage()->GetStageName())
			{
#if false
				DrawCircle(static_cast<int>(drawX + kStageSize * 0.5f), static_cast<int>(drawY + kStageSize * 0.5f), 16, 0xff8800, true);
#endif
				DrawRotaGraph(static_cast<int>(drawX + kStageSize * 0.5f), static_cast<int>(drawY + kStageSize * 0.5f),
					1.0, 0.0, m_nowPos->GetHandle(), true);
			}

			// MEMO:�A���`�G�C���A�X�t���ɂ���Ɛ��̑������w��\

			if (m_mgr.GetStage()->IsClearStage(stageName))
			{
				DrawBoxAA(static_cast<float>(drawX), static_cast<float>(drawY),
					static_cast<float>(drawX + kStageSize), static_cast<float>(drawY + kStageSize),
					0xffffff, false, kStageFrameThickness);
			}
			else
			{
				DrawBoxAA(static_cast<float>(drawX), static_cast<float>(drawY),
					static_cast<float>(drawX + kStageSize), static_cast<float>(drawY + kStageSize),
					0xff0000, false, kStageFrameThickness);

				DrawGraph(drawX + 16, drawY + 12, m_lock->GetHandle(), true);
			}
		}
	}
}
