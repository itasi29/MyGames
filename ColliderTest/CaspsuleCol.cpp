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
	// 自身の向いている方向に伸びているベクトルを作成
	Vec3 dirVec = m_dir.GetNormalized() * m_len * 0.5f;
	// 相対ベクトル
	Vec3 vec = circle.GetPos() - m_pos;

	// 相対ベクトルと正面ベクトルの内積
	float dot = Dot(vec, dirVec);
	// 方向ベクトルの大きさを取得(2乗)
	float sqLen = dirVec.SqLength();

	// 線分上のどこにあるかを確かめる
	float t = dot / sqLen;
	// 範囲の制限
	if (t < -1.0f) t = -1.0f; // 下限
	if (t > 1.0f) t = 1.0f; // 上限

	// 線分上での点までの最短座標
	Pos3 minPos = dirVec * t + m_pos;

	// 最短座標と円の座標のベクトル大きさ(2乗)を取得
	sqLen = (minPos - circle.GetPos()).SqLength();
	// 半径の合計の2乗
	float radius = m_radius + circle.GetRadius();
	radius = radius * radius;

	return sqLen < radius;
}
