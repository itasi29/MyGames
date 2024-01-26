#pragma once

#include "EnemyBase.h"

/// <summary>
/// �ʏ�̓G
/// </summary>
class EnemyNormal : public EnemyBase
{
public:
	EnemyNormal(const size& windowSize, float fieldSize);
	virtual ~EnemyNormal();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	void Init(const Vec2& pos) override;

protected:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;
};

