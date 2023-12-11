#pragma once
#include "EnemyBase.h"

class StageBase;

/// <summary>
/// EnemyChild�𐶐����Ă����G
/// </summary>
class EnemyCreate : public EnemyBase
{
public:
	EnemyCreate(const Size& windowSize, float fieldSize, StageBase* stage);
	virtual ~EnemyCreate();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	void Init(const Vec2& pos) override;

private:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;

private:
	StageBase* m_stage;

	// �ϊ���̈ړ��x�N�g��
	Vec2 m_conversionVec;

	// �p�x
	float m_radian;
};

