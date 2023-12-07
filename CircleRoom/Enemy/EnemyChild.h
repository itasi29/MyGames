#pragma once
#include "EnemyBase.h"

/// <summary>
/// ���g�̑����ɑҋ@������G
/// ���������Ԍo�߂ŏ�����s
/// </summary>
class EnemyChild : public EnemyBase
{
public:
	EnemyChild(const Size& windowSize, float fieldSize);
	virtual ~EnemyChild();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	void Init(Vec2& pos) override;

private:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;
};
