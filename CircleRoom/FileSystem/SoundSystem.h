#pragma once

/// <summary>
/// BGM,SEの再生を管理するシステム
/// </summary>

class SoundSystem
{
public:
	SoundSystem();

	/// <summary>
	/// BGMを流す
	/// </summary>
	/// <param name="soundHnadle">BGMハンドル</param>
	/// <param name="isLoop">ループするか(デフォでtrue)</param>
	void PlayBgm(int soundHnadle, bool isLoop = true);

	/// <summary>
	/// SEを流す
	/// </summary>
	/// <param name="seHandle">SEハンドル</param>
	/// <param name="playType">再生タイプ</param>
	void PlaySe(int seHandle, int playType);

	/// <summary>
	/// BGMのボリュームを変更する
	/// </summary>
	/// <param name="val">変更させる値 引数の値だけ±する</param>
	void ChangeBgmVol(int val);

	/// <summary>
	/// SEのボリュームを変更する
	/// </summary>
	/// <param name="val">変更させる値 引数の値だけ±する</param>
	void ChangeSeVol(int val);

	// Get関数
	int GetBgmVol() const { return m_bgmVolume; }
	int GetSeVol() const { return m_seVolume; }
	int GetMaxVol() const;
	/// <summary>
	/// BGMのボリュームの割合(0.0f ~ 1.0f)を取得
	/// </summary>
	/// <returns>BgmVolume</returns>
	float GetBgmVolRate() const;
	/// <summary>
	/// SEのボリュームの割合(0.0f ~ 1.0f)を取得
	/// </summary>
	/// <returns>SeVolum</returns>
	float GetSeVolRate() const;

private:

	/// <summary>
	/// 再生中のBGMの音量を変更する
	/// </summary>
	void SetBgm();
	
private:
	// ボリューム調整
	int m_bgmVolume;
	int m_seVolume;

	// 現在再生中のBGM
	int m_nowPlayBgm;
};

