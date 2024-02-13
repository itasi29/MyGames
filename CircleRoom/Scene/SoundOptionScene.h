#pragma once
#include "Scene.h"

class FileBase;
class SoundSystem;
class BottansFile;
class KeyFile;

/// <summary>
/// ���ʒ����V�[��
/// </summary>
class SoundOptionScene :
    public Scene
{
public:
    SoundOptionScene(GameManager& mgr, Input& input);
    ~SoundOptionScene();

    void Update(Input& input);
    void Draw() const;

private:
	using updateFunc_t = void(SoundOptionScene::*)(Input&);

	void NormalUpdate(Input& input);
	void EditUpdate(Input& input);

	void DrawName(int drawY, int index, std::wstring str) const;

	/// <summary>
	/// �Q�[�W�̕`��
	/// </summary>
	/// <param name="drawX">���[�`��ʒu</param>
	/// <param name="drawY">��[�`��ʒu</param>
	/// <param name="rate">�{�����[������</param>
	void DrawGauge(int drawX, int drawY, float rate) const;

	/// <summary>
	/// �I�𓙂̕����E�摜�`��
	/// </summary>
	/// /// <param name="cmd">�R�}���h��</param>
	/// <param name="str">�E�F�[�u�����镶����</param>
	/// <param name="num">������</param>
	void DrawWave(int x, int y, const char* const cmd, const wchar_t* const str[], int num) const;

private:
	updateFunc_t m_updateFunc;

	int m_currentLineIndex;
	bool m_isEdit;

	// �t�F�[�h�t���[��
	int m_fadeFrame;
	// ���s�[�g�m�F�t���[��
	int m_repeatUpFrame;
	int m_repeatDownFrame;

	// �摜�֌W
	std::shared_ptr<FileBase> m_frame;
	std::shared_ptr<FileBase> m_addFrame;
	std::shared_ptr<FileBase> m_startFrame;

	// ���֌W
	std::shared_ptr<SoundSystem> m_soundSys;
	// �I����Se
	std::shared_ptr<FileBase> m_selectSe;
	std::shared_ptr<FileBase> m_cursorSe;

	// �����E�F�[�u�p�̊p�x
	float m_waveAngle;

	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;

	const Input& m_input;
};

