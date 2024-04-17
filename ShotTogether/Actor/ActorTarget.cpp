#include "ActorTarget.h"
#include <DxLib.h>

namespace
{
	// アニメーション番号
	constexpr int kAnimIndexIdle = 1;
	constexpr int kAnimIndexRunRight = 8;
	constexpr int kAnimIndexRunLeft = 9;

	// 動く種類
	const enum Move : int
	{
		kIdle,
		kRight,
		kLeft,
		kMax
	};

	// 移動スピード
	constexpr float kMoveSpeed = 0.1f;
}

ActorTarget::ActorTarget(int handle, ActorManager& actorMgr, FileManager& fileMgr, SoundSystem& sndSys, const wchar_t* const name) :
	ActorBase(handle, actorMgr, fileMgr, sndSys, name),
	m_updateFunc(&ActorTarget::UpdateExsit)
{
}

ActorTarget::~ActorTarget()
{
}

void ActorTarget::Init(float x, float y)
{
}

void ActorTarget::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void ActorTarget::OnDeath()
{
}

void ActorTarget::UpdateExsit(Input& input)
{
	if (m_isAnimOneLoop)
	{
		ChangeMove();
	}
}

void ActorTarget::UpdateDeath(Input& input)
{
	if (m_isAnimOneLoop)
	{
		m_isExist = false;
	}
}

/// <summary>
/// 停止・左右移動の変更
/// </summary>
void ActorTarget::ChangeMove()
{
	// MEMO:現状は完全ランダムにしているが余裕があればある程度パターンを持たせる
	int type = GetRand(kMax - 1);

	if (type == kIdle)
	{
		ChangeAnim(kAnimIndexIdle);
		m_vel.x = 0.0f;
	}
	else if (type == kRight)
	{
		ChangeAnim(kAnimIndexRunRight);
		m_vel.x = kMoveSpeed;
	}
	else
	{
		ChangeAnim(kAnimIndexRunLeft);
		m_vel.x = -kMoveSpeed;
	}
}
