#include <DxLib.h>
#include "SoundSystem.h"

void SoundSystem::PlayBgm(int soundHnadle, int playType, bool isLoop)
{
	// �Đ����Ȃ疳��
	if (CheckSoundMem(soundHnadle)) return;

	// ���ʂ̕ύX
	ChangeNextPlayVolumeSoundMem(m_bgmVolume, soundHnadle);

	// ���[�v��ON�̏ꍇ��BGM���I������擪�ɖ߂�
	PlaySoundMem(soundHnadle, playType, isLoop);
}

void SoundSystem::PlaySe(int seHandle, int playType)
{
	// ���ʂ̕ύX
	ChangeNextPlayVolumeSoundMem(m_seVolume, seHandle);

	PlaySoundMem(seHandle, playType, true);
}

void SoundSystem::ChangeBgmVol(int val)
{
	m_bgmVolume += val;

	if (m_bgmVolume < 0)
	{
		m_bgmVolume = 0;
	}
	else if (m_bgmVolume > 255)
	{
		m_bgmVolume = 255;
	}

	SetBgm();
}

void SoundSystem::ChangeSeVol(int val)
{
	m_seVolume += val;

	if (m_seVolume < 0)
	{
		m_seVolume = 0;
		return;
	}
	if (m_seVolume > 255)
	{
		m_seVolume = 255;
		return;
	}
}

void SoundSystem::SetBgm()
{
	// �Đ����łȂ���ΏI��
	if (!CheckSoundMem(m_nowPlayBgm)) return;

	// �Y���Ă������萳�m�Ȃ������� GetSoundCurrentTime 
	int soundPosition = GetSoundCurrentPosition(m_nowPlayBgm);

	// ��x�T�E���h���~�߂�
	StopSoundMem(m_nowPlayBgm);

	// �Đ��ʒu��ݒ�
	SetSoundCurrentTime(soundPosition, m_nowPlayBgm);

	// ���ʂ̒���
	ChangeNextPlayVolumeSoundMem(m_bgmVolume, m_nowPlayBgm);

	// �ēx�Đ�
	PlaySoundMem(m_nowPlayBgm, DX_PLAYTYPE_BACK, false);
}
