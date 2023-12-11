#pragma once
#include "EnemyBase.h"
#include <memory>

class Player;

/// <summary>
/// �_�b�V�����Ă���G
/// </summary>
class EnemyDash : public EnemyBase
{
public:
	EnemyDash(const Size& windowSize, float fieldSize, std::shared_ptr<Player>& player);
	virtual ~EnemyDash();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	void Init(const Vec2& pos) override;

private:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;

	// �_�b�V���Ɋւ��鏈��
	void DashStartProcess();

private:
	std::shared_ptr<Player>& m_player;

	// ����_�b�V�����Ă��邩
	bool m_isDash;
	// �_�b�V������܂ł̑ҋ@�t���[��
	int m_waitDashFrame;
	// �_�b�V�����J�n����܂ł̃t���[��
	int m_startWaitDashFrame;
};

