#pragma once
#include <list>
#include <memory>
#include "BossBase.h"

class StageBase;
class BossDamageObject;

/// <summary>
/// ��ʃ{�X
/// </summary>
class BossArmored : public BossBase
{
public:
	BossArmored(const Size& windowSize, float fieldSize, int maxHp, StageBase* stage);
	virtual ~BossArmored();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	void Init(Vec2& pos) override;

	void OnAttack(bool isDash, const Collision& col) override;

protected:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;

	// �`��֐�
	void NormalDraw() const override;

	void CreateEnemy();

private:
	StageBase* m_stage;

	// ���ۂ̈ړ��͕ʂ̓����ƂȂ�̂ŕϐ��Ƃ��Ă����Ă���
	Vec2 m_conversionVec;

	// ���W�A��
	float m_radian;

	// �_���[�W��^���镨�̂̃��X�g
	std::list<std::shared_ptr<BossDamageObject>> m_objects;

	// �����Ԋu�t���[��
	int m_createFrame;
	// �����O�ҋ@�t���[��
	int m_waitCreateFrame;
};

