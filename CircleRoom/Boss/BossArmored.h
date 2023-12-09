#pragma once
#include <list>
#include "BossBase.h"

class BossArmored : public BossBase
{
public:
	BossArmored(const Size& windowSize, float fieldSize, int maxHp);
	virtual ~BossArmored();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	void Init(Vec2& pos) override;

	void OnAttack(const Collision& rect) override;

protected:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;

private:
	// ���ۂ̈ړ��͕ʂ̓����ƂȂ�̂ŕϐ��Ƃ��Ă����Ă���
	Vec2 m_conversionVec;

	// ���W�A��
	float m_radian;

	// �_���[�W��^����e�̃��X�g
};

