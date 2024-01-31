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
	BossArmored(const size& windowSize, float fieldSize);
	BossArmored(const size& windowSize, float fieldSize, StageBase* stage);
	virtual ~BossArmored();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	void Init(const Vec2& pos, bool isStart = false) override;

	/// <summary>
	/// �_���[�W����
	/// </summary>
	/// <param name="isDash">�v���C���[�̃_�b�V�����</param>
	/// <param name="col">�v���C���[�̓����蔻��</param>
	/// <returns>true: �_���[�W��^���� / false:�_���[�W��^���Ă��Ȃ�</returns>
	virtual bool OnAttack(bool isDash, const Collision& col) override;

protected:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;

	// �`��֐�
	virtual void NormalDraw() const override;

	virtual void CreateEnemy();

protected:
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

