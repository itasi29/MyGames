#pragma once
#include "EnemyBase.h"

/// <summary>
/// �ʏ���傫���T�C�Y�̓G
/// </summary>
class EnemyLarge : public EnemyBase
{
public:
	EnemyLarge(const Size& windowSize, float fieldSize);
	virtual ~EnemyLarge();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	void Init(const Vec2& pos) override;

private:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;
};

