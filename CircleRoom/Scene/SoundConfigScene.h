#pragma once
#include "Scene.h"

/// <summary>
/// ���ʒ����V�[��
/// </summary>
class SoundConfigScene :
    public Scene
{
public:
    SoundConfigScene(GameManager& mgr);
    ~SoundConfigScene();

    void Update(Input& input);
    void Draw();

private:
	// �X�V�����o�֐��|�C���^
	using UpdateFunc_t = void(SoundConfigScene::*)(Input& input);
	// �`�惁���o�֐��|�C���^
	using DrawFunc_t = void (SoundConfigScene::*)();

	// �X�V�֐�
	void AppearUpdate(Input&);	// �o����
	void NormalUpdate(Input&);		// �ʏ���
	void EditUpdate(Input&);		// �ҏW���
	void DisappearUpdate(Input&);	// �ޏ���

	// �`��֐�
	void ExpandDraw();	// �g���k���`��
	void NormalDraw();	// ��t�F�[�h�`��

	/// <summary>
	/// �Q�[�W�̕`��
	/// </summary>
	/// <param name="drawX">���[�`��ʒu</param>
	/// <param name="drawY">��[�`��ʒu</param>
	/// <param name="rate">�{�����[������</param>
	void DrawGauge(int drawX, int drawY, float rate);

private:
	UpdateFunc_t  m_updateFunc;
	DrawFunc_t m_drawFunc;

	int m_frame = 0;
};

