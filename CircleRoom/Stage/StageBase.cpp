#include <DxLib.h>
#include "Application.h"

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

void StageBase::UpdateSelect(Input& input)
{
	// �G�l�~�[�������������J��Ԃ�
	for (const auto& enemy : m_enemy)
	{
		enemy->Update();
	}

	// todo:A�{�^����B�{�^���������ꂽ��v���C���Ɉڍs����悤�ɂ���
}

void StageBase::UpdatePlaying(Input& input)
{
	// �v���C���[�̏��𔲂����
	bool playerIsDash = m_player->IsDash();
	bool playerIsExsit = m_player->IsExsit();
	const Rect playerRect = m_player->GetRect();

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

	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}

	// ���Ԃ̕`��
	int minSec = (m_frame * 100 / 60) % 100;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatString(m_windowSize.w / 2 - 32, 32, 0xffffff, L"%02d:%02d.%02d", min, sec, minSec);
}

void StageBase::DrawPlaying()
{
	m_player->Draw();
	for (const auto& enemy : m_enemy)
	{
		enemy->Draw();
	}

	// ���Ԃ̕`��
	int minSec = (m_frame * 100 / 60) % 100;
	int sec = (m_frame / 60) % 60;
	int min = m_frame / 3600;
	DrawFormatString(m_windowSize.w / 2 - 32, 32, 0xffffff, L"%02d:%02d.%02d", min, sec, minSec);
}
