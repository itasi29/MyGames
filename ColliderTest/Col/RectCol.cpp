#include "RectCol.h"
#include <cmath>

RectCol::RectCol()
{
}

RectCol::~RectCol()
{
}

void RectCol::Init(const Pos3& pos, const Size& size)
{
	m_pos = pos;
	m_size = size;
}

void RectCol::UpdatePos(const Pos3& pos)
{
	m_pos = pos;
}

bool RectCol::IsHit(const RectCol& rect)
{
	// ���΃x�N�g���̍쐬
	auto vec = rect.GetPos() - m_pos;
	// �l�̐�Βl��
	vec.x = fabs(vec.x);
	vec.y = fabs(vec.y);
	vec.z = fabs(vec.z);

	// ���ꂼ��̃T�C�Y�̔����̂��̂��쐬
	Size size = (rect.GetSize() + m_size) * 0.5f;

	// �e�����̔���
	bool isHitX = vec.x < size.w;
	bool isHitY = vec.y < size.h;
	bool isHitZ = vec.z < size.d;

	// ����
	return isHitX && isHitY && isHitZ;
}
