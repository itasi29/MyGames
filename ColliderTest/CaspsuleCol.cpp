#include "CaspsuleCol.h"
#include <cmath>
#include "CircleCol.h"

CaspsuleCol::CaspsuleCol()
{
}

CaspsuleCol::~CaspsuleCol()
{
}

void CaspsuleCol::Init(const Pos3& pos, const Vec3& dir, float len, float radius)
{
	m_pos = pos;
	m_dir = dir;
	m_len = len;
	m_radius = radius;
}

void CaspsuleCol::UpdatePos(const Pos3& pos, const Vec3& dir)
{
	m_pos = pos;
	m_dir = dir;
}

bool CaspsuleCol::IsHit(const CircleCol& circle)
{
	// ���g�̌����Ă�������ɐL�тĂ���x�N�g�����쐬
	Vec3 dirVec = m_dir.GetNormalized() * m_len * 0.5f;
	// ���΃x�N�g��
	Vec3 vec = circle.GetPos() - m_pos;

	// ���΃x�N�g���Ɛ��ʃx�N�g���̓���
	float dot = Dot(vec, dirVec);
	// �����x�N�g���̑傫�����擾(2��)
	float sqLen = dirVec.SqLength();

	// ������̂ǂ��ɂ��邩���m���߂�
	float t = dot / sqLen;
	// �͈͂̐���
	if (t < -1.0f) t = -1.0f; // ����
	if (t > 1.0f) t = 1.0f; // ���

	// ������ł̓_�܂ł̍ŒZ���W
	Pos3 minPos = dirVec * t + m_pos;

	// �ŒZ���W�Ɖ~�̍��W�̃x�N�g���傫��(2��)���擾
	sqLen = (minPos - circle.GetPos()).SqLength();
	// ���a�̍��v��2��
	float radius = m_radius + circle.GetRadius();
	radius = radius * radius;

	return sqLen < radius;
}
