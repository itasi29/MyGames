#include <DxLib.h>
#include <cassert>
#include "SoundSystem.h"

namespace
{
	constexpr int kMaxVolume = 255;
}

SoundSystem::SoundSystem() :
	m_bgmVolume(kMaxVolume),
	m_seVolume(kMaxVolume),
	m_nowPlayBgm(-1),
	m_soundHandle(-1)
{
}

void SoundSystem::PlayBgm(int soundHnadle, bool isLoop)
{
#ifdef _DEBUG
	auto result = CheckSoundMem(soundHnadle);

	// 再生中なら無視
	if (result == 1) return;

	if (result == -1) assert(false);
#else
	if (CheckSoundMem(soundHnadle)) return;
#endif

	// 音量の変更
	ChangeNextPlayVolumeSoundMem(m_bgmVolume, soundHnadle);

	// ループがONの場合はBGMが終了次第先頭に戻る
	PlaySoundMem(soundHnadle, DX_PLAYTYPE_BACK, isLoop);

	m_nowPlayBgm = soundHnadle;
}

void SoundSystem::PlaySe(int seHandle)
{
	// 音量の変更
	ChangeNextPlayVolumeSoundMem(m_seVolume, seHandle);

	PlaySoundMem(seHandle, DX_PLAYTYPE_BACK, true);
	m_soundHandle = seHandle;
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

	PlaySe(m_soundHandle);
}

int SoundSystem::GetMaxVol() const
{
	return kMaxVolume;
}

float SoundSystem::GetBgmVolRate() const
{
	return static_cast<float>(m_bgmVolume) / static_cast<float>(kMaxVolume);
}

float SoundSystem::GetSeVolRate() const
{
	return static_cast<float>(m_seVolume) / static_cast<float>(kMaxVolume);
}

void SoundSystem::SetBgm()
{
	// 再生中でなければ終了
	if (!CheckSoundMem(m_nowPlayBgm)) return;

	// ズレていたらより正確なこっちで GetSoundCurrentTime 
	LONGLONG soundPosition = GetSoundCurrentPosition(m_nowPlayBgm);

	// 一度サウンドを止める
	StopSoundMem(m_nowPlayBgm);

	// 再生位置を設定
	SetSoundCurrentTime(soundPosition, m_nowPlayBgm);

	// 音量の調整
	ChangeNextPlayVolumeSoundMem(m_bgmVolume, m_nowPlayBgm);

	// 再度再生
	PlaySoundMem(m_nowPlayBgm, DX_PLAYTYPE_BACK, false);
}
