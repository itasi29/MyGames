#pragma once
#include "EnemyBase.h"
class EnemyMoveWall : public EnemyBase
{
public:
	EnemyMoveWall(const Size& windowSize, float fieldSize);
	virtual ~EnemyMoveWall();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="vec">������ (��or��)</param>
	void Init(Vec2& vec) override;

private:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;

	// �`��֐�
	void StartDraw() override;
	void NormalDraw() override;
};

