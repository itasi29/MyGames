#pragma once
#include "EnemyBase.h"

class StageBase;

/// <summary>
/// ���􂵂�4�����ɔ�΂��G
/// </summary>
class EnemyDivision : public EnemyBase
{
public:
	EnemyDivision(const size& windowSize, float fieldSize);
	EnemyDivision(const size& windowSize, float fieldSize, StageBase* stage);
	virtual ~EnemyDivision();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	void Init(const Vec2& pos, bool isStart = false) override;

private:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;

private:
	StageBase* m_stage;

	// �����ҋ@�O��
	bool m_isDivisionWait;
	// �����O�ҋ@�t���[��
	int m_divisionWaitFrame;
};

