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
	void Init(Vec2& pos) override;

private:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;

private:
	StageBase* m_stage;

	// �p�x
	float m_radian;
};

