#pragma once

struct size
{
	int w;
	int h;
};

/// <summary>
/// �A�v���P�[�V�����S�̂��Ǘ�����N���X
/// �V���O���g���N���X
/// </summary>
class Application
{
private:
	Application();	// �V���O���g���̂��߂�private��
	// �R�s�[������̋֎~
	Application(const Application& app) = delete;		// �R�s�[�R���X�g���N�^�̔p�~
	void operator =(const Application& app) = delete;	// ����R���X�g���N�^�̔p�~

public:
	/// <summary>
	/// Application�̃V���O���g���C���X�^���X��Ԃ�
	/// </summary>
	/// <returns>Application�C���X�^���X�̎Q�Ƃ�Ԃ�</returns>
	static Application& GetInstance()
	{
		static Application instance;	// �������g�̐ÓI�I�u�W�F�N�g�����܂��B
		return instance;
	}

	bool Init();
	void Run();
	/// <summary>
	/// �㏈�����s��
	/// </summary>
	void Terminate();

	/// <summary>
	/// �Q�[���̏I��
	/// </summary>
	void End();

	const size& GetWindowSize() const;

	/// <summary>
	/// �E�B���h���[�h�̐؂�ւ�
	/// </summary>
	void ChangeWindows();

	/// <summary>
	/// ���݂̃E�B���h���[�h�̏�Ԃ��擾
	/// </summary>
	/// <returns>�E�B���h�E���[�h</returns>
	bool IsWindows() const { return m_isWindows; }

private:
	size m_size;
	// FPS�Œ�p
	long long m_time;

	// �Q�[���I���t���O
	bool m_isEnd;

	// �E�B���h�E�t���O
	bool m_isWindows;
};

