#pragma once
#include "EnemyBase.h"
#include <vector>
#include <memory>

class Player;

/// <summary>
/// �_�b�V�����Ă���G
/// </summary>
class EnemyDash : public EnemyBase
{
public:
	EnemyDash(const size& windowSize, float fieldSize);
	EnemyDash(const size& windowSize, float fieldSize, std::shared_ptr<Player>& player);
	virtual ~EnemyDash();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	void Init(const Vec2& pos, bool isStart = false) override;

private:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;

	void NormalDraw() const override;

	// �_�b�V���Ɋւ��鏈��
	void Dash();

	void DrawDashEff() const;

private:
	// �v���C���[�̃|�C���^
	std::shared_ptr<Player> m_player;

	// �_�b�V��Se
	std::shared_ptr<FileBase> m_dashSe;

	// �ʒu���O
	std::vector<Vec2> m_posLog;
	// ���O�t���[��
	int m_logFrame;
	// ����_�b�V�����Ă��邩
	bool m_isDash;
	// �_�b�V������܂ł̑ҋ@�t���[��
	int m_waitDashFrame;
	// �_�b�V�����J�n����܂ł̃t���[��
	int m_startWaitDashFrame;

	// �_�b�V���̈ʒu
	Vec2 m_dashEffPos;
	// �_�b�V�����̔g��p
	int m_dashEffRipper;
	// �g��`�撆��
	bool m_isDashEff;
	// �_�b�V�����̕`���
	int m_dashEffScreen;

	// �k�܂��邩
	bool m_isShrink;
	// �k�܂���
	double m_circleSize;
};

