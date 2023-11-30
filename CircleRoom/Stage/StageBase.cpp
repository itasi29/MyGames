#include <DxLib.h>
#include "Application.h"
#include "Input.h"

#include "StageBase.h"
#include "StageManager.h"

#include "Player/Player.h"
#include "Enemy/EnemyBase.h"

StageBase::StageBase(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize) :
	m_mgr(mgr),
	m_windowSize(windowSize),
	m_fieldSize(fieldSize),
	m_frame(0)
{
	m_updateFunc = &StageBase::UpdateSelect;
	m_drawFunc = &StageBase::DrawSelect;
}

StageBase::~StageBase()
{
}

void StageBase::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void StageBase::Draw()
{
	(this->*m_drawFunc)();
}

void StageBase::DrawStageConditions()
{
}

void StageBase::UpdateSelect(Input& input)
{
	// �G�l�~�[�������������J��Ԃ�
	for (const auto& enemy : m_enemy)
	{
		enemy->Update();
	}

	if (input.IsPress("OK"))
	{
		// �����o�֐��|�C���^�̍X�V
		m_updateFunc = &StageBase::UpdatePlaying;
		m_drawFunc = &StageBase::DrawPlaying;

		// �e�평��������
		Init();
	}
}

void StageBase::UpdatePlaying(Input& input)
{
	// �v���C���[�̏��𔲂����
	bool playerIsDash = m_player->IsDash();
	bool playerIsExsit = m_player->IsExsit();
	const Rect playerRect = m_player->GetRect();

	CreateEnemy();
	for (const auto& enemy : m_enemy)
	{
		enemy->Update();

		// �v���C���[�Ƃ̓����蔻�菈��
		// �_�b�V�����Ă����珈���͂��Ȃ�
		if (!playerIsDash && playerRect.IsCollsion(enemy->GetRect()))
		{
			// �v���C���[�̎��S����
			m_player->Death();

			// �����o�֐��|�C���^��I���̕��ɖ߂�
			m_updateFunc = &StageBase::UpdateSelect;
			m_drawFunc = &StageBase::DrawSelect;
		}
	}

	// �o�ߎ��Ԃ̍X�V
	m_frame++;
	m_player->Update(input);
}

void StageBase::DrawSelect()
{
	// todo:�`��͍��̂Ƃ��듯�������A����͕ς���悤�ɂ���

	DrawWall();

	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}

	// �X�e�[�W���̕`��
	DrawFormatString(128, 16, 0xffffff, L"%s", m_stageName.c_str());
	// ���Ԃ̕`��
	DrawFormatString(128, 32, 0xffffff, L"00:00.000");
	// �X�e�[�W�����̕`��
	/*�ꎞ�I�ɏ����Ƃ����`��*/
	//DrawStageConditions();
	DrawString(128, 48, L"����", 0xffffff);
}

void StageBase::DrawPlaying()
{
	DrawWall();

	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}

	// ���Ԃ̕`��
	int minSec = (m_frame * 1000 / 60) % 1000;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatString(128, 32, 0xffffff, L"%02d:%02d.%03d", min, sec, minSec);
}

void StageBase::DrawWall()
{
	float centerX = m_windowSize.w * 0.5f;
	float centerY = m_windowSize.h * 0.5f;

	// �F�͉�
	// ��
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0x00ff00);
	// �E
	DrawLine(static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0x00ff00);
	// ��
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY - m_fieldSize),
		0x00ff00);
	// ��
	DrawLine(static_cast<int>(centerX - m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		static_cast<int>(centerX + m_fieldSize), static_cast<int>(centerY + m_fieldSize),
		0x00ff00);
}
