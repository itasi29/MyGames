#include "ActorTarget.h"
#include <DxLib.h>

namespace
{
	// �A�j���[�V�����ԍ�
	constexpr int kAnimIndexIdle = 1;
	constexpr int kAnimIndexRunRight = 8;
	constexpr int kAnimIndexRunLeft = 9;

	// �������
	const enum Move : int
	{
		kIdle,
		kRight,
		kLeft,
		kMax
	};

	// �ړ��X�s�[�h
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
/// ��~�E���E�ړ��̕ύX
/// </summary>
void ActorTarget::ChangeMove()
{
	// MEMO:����͊��S�����_���ɂ��Ă��邪�]�T������΂�����x�p�^�[������������
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
