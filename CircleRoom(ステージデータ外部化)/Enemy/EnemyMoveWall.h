#pragma once
#include "EnemyBase.h"

/// <summary>
/// �ǂɉ����ē����G
/// </summary>
class EnemyMoveWall : public EnemyBase
{
public:
	EnemyMoveWall(const size& windowSize, float fieldSize);
	virtual ~EnemyMoveWall();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="vec">������ (��or��)</param>
	void Init(const Vec2& vec, bool isStart = false) override;

private:
	// �X�V�֐�
	void StartUpdate() override;
	void NormalUpdate() override;
};

