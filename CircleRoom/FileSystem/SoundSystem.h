#pragma once

/// <summary>
/// BGM,SE�̍Đ����Ǘ�����V�X�e��
/// </summary>

class SoundSystem
{
public:
	SoundSystem();

	/// <summary>
	/// BGM�𗬂�
	/// </summary>
	/// <param name="soundHnadle">BGM�n���h��</param>
	/// <param name="isLoop">���[�v���邩(�f�t�H��true)</param>
	void PlayBgm(int soundHnadle, bool isLoop = true);

	/// <summary>
	/// SE�𗬂�
	/// </summary>
	/// <param name="seHandle">SE�n���h��</param>
	/// <param name="playType">�Đ��^�C�v</param>
	void PlaySe(int seHandle, int playType);

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
	void SetBgm();
	
private:
	// �{�����[������
	int m_bgmVolume;
	int m_seVolume;

	// ���ݍĐ�����BGM
	int m_nowPlayBgm;
};

