#include <DxLib.h>
#include "SoundSystem.h"

void SoundSystem::PlayBgm(int soundHnadle, int playType, bool isLoop)
{
	// 再生中なら無視
	if (CheckSoundMem(soundHnadle)) return;

	// 音量の変更
	ChangeNextPlayVolumeSoundMem(m_bgmVolume, soundHnadle);

	// ループがONの場合はBGMが終了次第先頭に戻る
	PlaySoundMem(soundHnadle, playType, isLoop);
}

void SoundSystem::PlaySe(int seHandle, int playType)
{
	// 音量の変更
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
	// 再生中でなければ終了
	if (!CheckSoundMem(m_nowPlayBgm)) return;

	// ズレていたらより正確なこっちで GetSoundCurrentTime 
	int soundPosition = GetSoundCurrentPosition(m_nowPlayBgm);

	// 一度サウンドを止める
	StopSoundMem(m_nowPlayBgm);

	// 再生位置を設定
	SetSoundCurrentTime(soundPosition, m_nowPlayBgm);

	// 音量の調整
	ChangeNextPlayVolumeSoundMem(m_bgmVolume, m_nowPlayBgm);

	// 再度再生
	PlaySoundMem(m_nowPlayBgm, DX_PLAYTYPE_BACK, false);
}
