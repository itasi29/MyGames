#pragma once
#include <list>
#include <memory>
#include "Scene.h"

class FileBase;
class SoundSystem;
class EnemyBase;
class BossBase;
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

	// �G�̐����֐�
	void CreateEnemy();
	// �{�X�̐����֐�
	void CreateBoss();

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
	std::shared_ptr<FileBase> m_logo;
	std::shared_ptr<FileBase> m_bg;
	std::shared_ptr<FileBase> m_frame;

	// �^�C�g���œ����G
	std::list<std::shared_ptr<EnemyBase>> m_enemy;
	std::list<std::shared_ptr<BossBase>> m_boss;

	// �^�C�g���œ����G�̐����t���[��
	int m_createEnemyFrame;
	int m_createBossFrame;
	// �G�̐����^�C�~���O
	int m_createEnemyTiming;
	int m_createBossTiming;

	// �S�̃t���[��
	int m_fadeFrame;
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

