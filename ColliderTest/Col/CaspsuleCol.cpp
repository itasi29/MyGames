#include "CaspsuleCol.h"
#include <cmath>
#include <algorithm>
#include "SphereCol.h"
#include "Matrix3x3.h"

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

bool CaspsuleCol::IsHit(const SphereCol& circle)
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

bool CaspsuleCol::IsHit(const CaspsuleCol& col)
{
	// ���g�̌����Ă�������ɐL�тĂ���x�N�g�����쐬
	Vec3 sDirVec = m_dir.GetNormalized() * m_len * 0.5f;
	// �Ώۂ̌����Ă�������ɐL�тĂ���x�N�g�����쐬
	Vec3 tDirVec = col.GetDir().GetNormalized() * col.GetLength() * 0.5f;

	// ���΃x�N�g��
	Vec3 vec = col.GetPos() - m_pos;

	// �@���x�N�g��
	Vec3 norm = Cross(sDirVec, tDirVec);
	// ���s����
	bool isParallel = norm.SqLength() < 0.001f;

	float s, t;
	// ���s�łȂ��ꍇ
	if (!isParallel)
	{
		// �P�ʍs��
		Matrix3x3 mat;
		mat.Identity();

		// �l�̑��
		mat.SetLine(0, sDirVec);
		mat.SetLine(1, tDirVec.Reverse());
		mat.SetLine(2, norm);

		// �t�s��
		mat = mat.GetInverse();

		s = Dot(mat.GetRow(0), vec);
		t = Dot(mat.GetRow(1), vec);
	}
	// ���s�̏ꍇ
	else
	{
		s = Dot(sDirVec, vec) / sDirVec.SqLength();
		t = Dot(tDirVec, vec) / tDirVec.SqLength();
	}

	// �͈͂̐���
	if (s < -1.0f) s = -1.0f; // ����
	if (s > 1.0f)  s =  1.0f; // ���
	if (t < -1.0f) t = -1.0f; // ����
	if (t > 1.0f)  t =  1.0f; // ���

	// ������ł̍ŒZ���W
	Pos3 minPos1 = sDirVec * s + m_pos;
	Pos3 minPos2 = tDirVec * t + col.GetPos();
	// �傫��(2��)
	float sqLen = (minPos1 - minPos2).SqLength();
	// ���ꂼ��̔��a�̍��v��2��
	float ar = m_radius + col.GetRadius();
	ar = ar * ar;
		
	return sqLen < ar;
}
