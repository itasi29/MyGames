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

	void NormalDraw() const override;

	void UsuallyUpdate();
	void DivisionUpdate();
	void EndUpdate();

	void UsuallyDraw() const;
	void EndDraw() const;

private:
	using UpdateFunc_t = void(EnemyDivision::*)();
	using DrawFunc_t = void(EnemyDivision::*)() const;

	UpdateFunc_t m_dUpdateFunc;
	DrawFunc_t m_dDrawFunc;

	StageBase* m_stage;

	// �����O�ҋ@�t���[��
	int m_divisionWaitFrame;

	// �j��p
	int m_ripple;
	int m_rippleScreen;
};

