#pragma once
#include <memory>
#include "Scene.h"

class FileBase;
class SoundSystem;
struct size;

/// <summary>
/// �^�C�g���V�[���N���X
/// </summary>
class TitleScene : public Scene
{
public:
	TitleScene(GameManager& mgr);
	~TitleScene();
	virtual void Update(Input& input);
	virtual void Draw();

private:
	// �X�V�֐�
	void FadeInUpdate(Input&);	// �t�F�[�h�C�����
	void NormalUpdate(Input&);	// �ʏ���
	void StartSelectUpdate(Input&);	// ��������or���߂���
	void FadeOutUpdate(Input&);	// �t�F�[�h�A�E�g���

	// �`��֐�
	void FadeDraw();	// �t�F�[�h���`��
	void NormalDraw();	// ��t�F�[�h�`��
	void StartSelectDraw();	// �X�^�[�g�I����ԕ`��

private:
	/// <summary>
	/// ���S�̕`��
	/// </summary>
	void DrawLogo();

private:
	// �X�V�����o�֐��|�C���^
	void(TitleScene::* m_updateFunc)(Input& input);
	// �`�惁���o�֐��|�C���^
	using DrawFunc_t = void (TitleScene::*)();
	DrawFunc_t m_drawFunc;

	// �摜�Q
	std::shared_ptr<FileBase> m_logoImg;
	std::shared_ptr<FileBase> m_bgImg;

	// �S�̃t���[��
	int m_frame;
	// ���j���[�I���J�[�\���ʒu
	int m_currentLinePos;

	// ���S�A���O��
	float m_logoAngle;

	// �w�i�t���[��
	int m_bgFrame;

	// ���֌W
	std::shared_ptr<SoundSystem> m_soundSys;
	// Bgm
	std::shared_ptr<FileBase> m_bgm;
	// �I����Se
	std::shared_ptr<FileBase> m_selectSe;
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;
};

