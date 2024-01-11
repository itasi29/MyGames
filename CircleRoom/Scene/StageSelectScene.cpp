#include <DxLib.h>
#include <string>

#include "Vec2.h"
#include "GameManager.h"
#include "Stage/StageManager.h"
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
	constexpr int kStageFrameSize = 16;
	// �t���[���̑���
	constexpr int kStageFrameThickness = 4;
	// �X�e�[�W�Ԃ̋�
	constexpr int kStageMargine = 96;

	// �X�^�[�g�ʒu
	const Vec2 kStartPos(kMenuMargin + kStageMargine, kMenuMargin + kStageMargine);

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
	m_indexLine(0)
{
}

StageSelectScene::~StageSelectScene()
{
}

void StageSelectScene::Update(Input& input)
{
	// �㉺�̃C���f�b�N�X�̕ύX
	if (input.IsTriggered("up"))
	{
		m_indexLine = (m_indexLine - 1 + kLineNum) % kLineNum;
	}
	if (input.IsTriggered("down"))
	{
		m_indexLine = (m_indexLine + 1) % kLineNum;
	}

	// ���E�̃C���f�b�N�X�̕ύX
	if (input.IsTriggered("left"))
	{
		m_indexRow = (m_indexRow - 1 + kRowNum) % kRowNum;
	}
	if (input.IsTriggered("right"))
	{
		m_indexRow = (m_indexRow + 1) % kRowNum;
	}

	if (input.IsTriggered("OK"))
	{
		// �X�e�[�W���Ȃ��Ȃ�I��
		if (kStageStr[m_indexLine][m_indexRow] == "None") return;
		auto stgName = "Stage" + kStageStr[m_indexLine][m_indexRow];

		// ���̃X�e�[�W���N���A����Ă��Ȃ���ΏI��
		if (!m_mgr.GetStage()->IsClearStage(stgName)) return;


	}
}

void StageSelectScene::Draw()
{
	DrawString(100, kMenuMargin + 10, L"StagSelectScene", 0xffffff);

	for(int x = 0; x < kRowNum; x++)
	{
		int drawX = kStartPos.x + kStageMargine * x;
		bool isIndexRow = (m_indexRow == x);

		for (int y = 0; y < kLineNum; y++)
		{
			if (isIndexRow && m_indexLine == y)
			{
				// FIXE:����傫�߂̘g��`�悵�Ă��邪�̂��͉摜�ɕύX
				DrawBoxAA(drawX - kStageFrameSize, kStartPos.y + kStageMargine * y - kStageFrameSize,
					drawX + kStageSize + kStageFrameSize, kStartPos.y + kStageMargine * y + kStageSize + kStageFrameSize,
					0xffff00, false, kStageFrameThickness);
			}

			// �X�e�[�W�̕`�悪�����̏ꍇ�͈ȉ��͕̂`�悵�Ȃ�
			if (kStageStr[y][x] == "None") continue;

			int drawY = kStartPos.y + kStageMargine * y;

			// ���ݑI�����Ă���X�e�[�W
			// FIXME:���Ƃ��ĉ~��`�悵�Ă���̂ŁA�O���t�B�b�N���o������ύX
			if (("Stage" + kStageStr[y][x]) == m_mgr.GetStage()->GetStageName())
			{
				DrawCircle(drawX + kStageSize * 0.5f, drawY + kStageSize * 0.5f, 16, 0xff8800, true);
			}

			//for (int i = 0; i < kStageFrameThickness; i++)
			//{
			//	DrawBox(drawX + i, kStartPos.y + kStageMargine * y + i,
			//		drawX + kStageSize - i, kStartPos.y + kStageMargine * y + kStageSize - i,
			//		0xffffff, false);
			//}

			// MEMO:�A���`�G�C���A�X�t���ɂ���Ɛ��̑������w��\
			DrawBoxAA(drawX, drawY,
				drawX + kStageSize, drawY + kStageSize,
				0xffffff, false, kStageFrameThickness);
		}
	}
}
