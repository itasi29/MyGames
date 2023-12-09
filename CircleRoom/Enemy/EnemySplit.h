#pragma once
#include "EnemyBase.h"

/// <summary>
/// �����̓G
/// </summary>
class EnemySplit : public EnemyBase
{
public:
	EnemySplit(const Size& windowSize, float fieldSize);
	virtual ~EnemySplit();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="vec">�ړ��x�N�g��</param>
	void Init(Vec2& pos, Vec2& vec);

private:
	// �X�V�֐�
	void StartUpdate() override;
	virtual void NormalUpdate() override;
};

