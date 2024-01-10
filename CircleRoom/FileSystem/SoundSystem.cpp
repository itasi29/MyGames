#include <DxLib.h>
#include <cassert>
#include "SoundSystem.h"

namespace
{
	constexpr int kMaxVolume = 255;
}

SoundSystem::SoundSystem() :
	m_bgmVolume(kMaxVolume),
	m_seVolume(kMaxVolume)
{
}

void SoundSystem::PlayBgm(int soundHnadle, bool isLoop)
{
#ifdef _DEBUG
	auto result = CheckSoundMem(soundHnadle);

	// �Đ����Ȃ疳��
	if (result == 1) return;

	if (result == -1) assert(false);
#else
	if (CheckSoundMem(soundHnadle)) return;
#endif

	// ���ʂ̕ύX
	ChangeNextPlayVolumeSoundMem(m_bgmVolume, soundHnadle);

	// ���[�v��ON�̏ꍇ��BGM���I������擪�ɖ߂�
	PlaySoundMem(soundHnadle, DX_PLAYTYPE_BACK, isLoop);

	m_nowPlayBgm = soundHnadle;
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
	else if (m_bgmVolume > kMaxVolume)
	{
		m_bgmVolume = kMaxVolume;
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
	if (m_seVolume > kMaxVolume)
	{
		m_seVolume = kMaxVolume;
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
