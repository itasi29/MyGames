#pragma once
#include "EnemyBase.h"

class Stage;

/// <summary>
/// EnemyChild�𐶐����Ă����G
/// </summary>
class EnemyCreate : public EnemyBase
{
public:
	EnemyCreate(const size& windowSize, float fieldSize);
	EnemyCreate(const size& windowSize, float fieldSize, Stage* stage);
	virtual ~EnemyCreate();

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
	// �X�e�[�W�̃|�C���^
	Stage* m_stage;

	// �ϊ���̈ړ��x�N�g��
	Vec2 m_conversionVec;

	// �p�x
	float m_radian;
};

