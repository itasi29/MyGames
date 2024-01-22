#pragma once

/// <summary>
/// BGM,SE�̍Đ����Ǘ�����V�X�e��
/// </summary>

class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	void Init();
	void End();

	/// <summary>
	/// BGM�𗬂�
	/// </summary>
	/// <param name="soundHnadle">BGM�n���h��</param>
	/// <param name="isLoop">���[�v���邩(�f�t�H��true)</param>
	void PlayBgm(int soundHnadle, bool isLoop = true);

	/// <summary>
	/// Fade�ݒ肠��BGM
	/// </summary>
	/// <param name="soundHandle">BGM�n���h��</param>
	/// <param name="val">���ʃp�[�Z���g</param>
	/// <param name="isLoop">���[�v���邩</param>
	void PlayFadeBgm(int soundHandle, float val = 1.0f, bool isLoop = true);

	/// <summary>
	/// SE�𗬂�
	/// </summary>
	/// <param name="seHandle">SE�n���h��</param>
	void PlaySe(int seHandle);

	/// <summary>
	/// �����̒�~
	/// </summary>
	/// /// <param name="soundHandle">�n���h��</param>
	void Stop(int soundHandle = -1);

	/// <summary>
	/// BGM�̃{�����[����ύX����
	/// </summary>
	/// <param name="val">�ύX������l �����̒l�����}����</param>
	void ChangeBgmVol(int val);

	/// <summary>
	/// SE�̃{�����[����ύX����
	/// </summary>
	/// <param name="val">�ύX������l �����̒l�����}����</param>
	void ChangeSeVol(int val);

	// Get�֐�
	int GetBgmVol() const { return m_bgmVolume; }
	int GetSeVol() const { return m_seVolume; }
	int GetMaxVol() const;
	/// <summary>
	/// BGM�̃{�����[���̊���(0.0f ~ 1.0f)���擾
	/// </summary>
	/// <returns>BgmVolume</returns>
	float GetBgmVolRate() const;
	/// <summary>
	/// SE�̃{�����[���̊���(0.0f ~ 1.0f)���擾
	/// </summary>
	/// <returns>SeVolum</returns>
	float GetSeVolRate() const;

private:

	/// <summary>
	/// �Đ�����BGM�̉��ʂ�ύX����
	/// </summary>
	void SetBgm(float rate = 1.0f);
	
private:
	// �{�����[������
	int m_bgmVolume;
	int m_seVolume;

	// ���ݍĐ�����BGM
	int m_nowPlayBgm;
	// �Ō�ɖ炵��SE
	int m_soundHandle;
};

