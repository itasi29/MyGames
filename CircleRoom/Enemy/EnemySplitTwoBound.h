#pragma once
#include "EnemySplit.h"
class EnemySplitTwoBound : public EnemySplit
{
public:
	EnemySplitTwoBound(const Size& windowSize, float fieldSize);
	virtual ~EnemySplitTwoBound();

private:
	void NormalUpdate() override;

private:
	int m_boundNo;
};

