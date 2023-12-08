#pragma once

#include "EnemyBase.h"

/// <summary>
/// �ʏ�̓G
/// </summary>
class EnemyNormal : public EnemyBase
{
public:
	EnemyNormal(const Size& windowSize, float fieldSize);
	virtual ~EnemyNormal();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	void Init(Vec2& pos) override;

protected:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;
};

