#include <DxLib.h>
#include <cassert>
#include <algorithm>
#include "SoundSystem.h"

namespace
{
	constexpr int kMaxVolume = 255;
}

SoundSystem::SoundSystem() :
	m_bgmVolume(kMaxVolume),
	m_seVolume(kMaxVolume),
	m_nowPlayBgm(-1),
	m_soundHandle(-1),
	m_soundSavePos(0)
{
}

SoundSystem::~SoundSystem()
{
}

SoundSystem& SoundSystem::GetInstance()
{
	static SoundSystem snd;
	return snd;
}

void SoundSystem::PlayBgm(int soundHandle, bool isLoop, bool isSoundPosSave)
{
	if (soundHandle < 0)
	{
		soundHandle = m_nowPlayBgm;
	}

#ifdef _DEBUG
	auto result = CheckSoundMem(soundHandle);

	// 再生中なら無視
	if (result == 1) return;

	if (result == -1) assert(false);
#else
	if (CheckSoundMem(soundHandle)) return;
#endif

	// 音量の変更
	ChangeNextPlayVolumeSoundMem(m_bgmVolume, soundHandle);

	if (isSoundPosSave)
	{
		SetSoundCurrentTime(m_soundSavePos, soundHandle);
		PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK, isLoop);
	}
	else
	{
		PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK, isLoop);
	}

	m_nowPlayBgm = soundHandle;
}

void SoundSystem::PlayFadeBgm(int soundHandle, float rate, bool isLoop)
{
	if (soundHandle < 0)
	{
		soundHandle = m_nowPlayBgm;
	}

	if (CheckSoundMem(soundHandle))
	{
		SetBgmVol(rate);
		return;
	}

	// 音量の変更
	ChangeNextPlayVolumeSoundMem(static_cast<int>(m_bgmVolume * rate), soundHandle);

	// ループがONの場合はBGMが終了次第先頭に戻る
	PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK, isLoop);

	m_nowPlayBgm = soundHandle;
}

void SoundSystem::PlaySe(int seHandle)
{
	// 音量の変更
	ChangeNextPlayVolumeSoundMem(m_seVolume, seHandle);

	PlaySoundMem(seHandle, DX_PLAYTYPE_BACK, true);
//	PlaySoundMem(seHandle, DX_PLAYTYPE_NORMAL, true);
	m_soundHandle = seHandle;
}

void SoundSystem::Stop(int soundHandle, bool isSoundPosSave)
{
	if (soundHandle < 0)
	{
		soundHandle = m_nowPlayBgm;
	}

	StopSoundMem(soundHandle);

	// 音声再生場所の保存
	if (isSoundPosSave)
	{
		m_soundSavePos = GetSoundCurrentPosition(soundHandle);
	}
}

void SoundSystem::ChangeBgmVol(int val)
{
	m_bgmVolume += val;

	m_bgmVolume = std::max<int>(std::min<int>(kMaxVolume, m_bgmVolume), 0);

	SetBgmVol();
}

void SoundSystem::ChangeSeVol(int val)
{
	m_seVolume += val;

	m_bgmVolume = std::max<int>(std::min<int>(kMaxVolume, m_seVolume), 0);

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

void SoundSystem::SetBgmVol(float rate)
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
	ChangeNextPlayVolumeSoundMem(static_cast<int>(m_bgmVolume * rate), m_nowPlayBgm);

	// 再度再生
	PlaySoundMem(m_nowPlayBgm, DX_PLAYTYPE_BACK, false);
}
