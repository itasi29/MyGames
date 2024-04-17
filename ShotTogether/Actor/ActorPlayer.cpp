#include "ActorPlayer.h"
#include "Common/Input.h"

namespace
{
	// �A�j���[�V�����ԍ�
	constexpr int kAnimIndexIdle = 1;
	constexpr int kAnimIndexRunRight = 8;
	constexpr int kAnimIndexRunLeft = 8;
	constexpr int kAnimIndexShot = 53;
	constexpr int kAnimIndexReload = 55;

	// �ړ����x
	constexpr float kSpeed = 0.1f;
}

ActorPlayer::ActorPlayer(int handle, ActorManager& actorMgr, FileManager& fileMgr, SoundSystem& sndSys, const wchar_t* const name) :
	ActorBase(handle, actorMgr, fileMgr, sndSys, name),
	m_updateFunc(&ActorPlayer::UpdateExsit)
{
	ChangeAnim(kAnimIndexIdle);
}

ActorPlayer::~ActorPlayer()
{
}

void ActorPlayer::Init(float x, float y)
{
	ActorBase::Init(x, y);
}

void ActorPlayer::Update(Input& input)
{
	(this->*m_updateFunc)(input);

	ActorBase::Update(input);
}

/// <summary>
/// �ҋ@
/// </summary>
/// <param name="input">���͏��</param>
void ActorPlayer::UpdateExsit(Input& input)
{
	// �ړ���ԂɑJ��
	if (input.IsTriggered("right"))
	{
		OnMove(true);
	}
	if (input.IsTriggered("left"))
	{
		OnMove(false);
	}
	// �e�̔���
	if (input.IsTriggered("shot"))
	{
		OnShot();
	}
}

void ActorPlayer::UpdateMoveRight(Input& input)
{
	if (input.IsReleased("right"))
	{
		OnNeutual();
	}
	if (input.IsTriggered("left"))
	{
		OnMove(false);
	}
}

void ActorPlayer::UpdateMoveLeft(Input& input)
{
	if (input.IsReleased("left"))
	{
		OnNeutual();
	}
	if (input.IsTriggered("right"))
	{
		OnMove(true);
	}
}

/// <summary>
/// �e�̔���
/// </summary>
/// <param name="input">���͏��</param>
void ActorPlayer::UpdateShot(Input& input)
{
	if (m_isAnimOneLoop)
	{
		OnReload();
	}
}

/// <summary>
/// �����[�h
/// </summary>
/// <param name="input">���͏��</param>
void ActorPlayer::UpdateReload(Input& input)
{
	if (m_isAnimOneLoop)
	{
		OnNeutual();
	}
}

/// <summary>
/// �ҋ@��ԂɑJ��
/// </summary>
void ActorPlayer::OnNeutual()
{
	m_vel = { 0.0f, 0.0f, 0.0f };

	ChangeAnim(kAnimIndexIdle);
	m_updateFunc = &ActorPlayer::UpdateExsit;
}

/// <summary>
/// �ړ���ԂɑJ��
/// </summary>
/// <param name="isRight">true:�E�ɓ��� / false:���ɓ���</param>
void ActorPlayer::OnMove(bool isRight)
{
	if (isRight)
	{
		m_vel.x = kSpeed;
		ChangeAnim(kAnimIndexRunRight);
		m_updateFunc = &ActorPlayer::UpdateMoveRight;
	}
	else
	{
		m_vel.x = -kSpeed;
		ChangeAnim(kAnimIndexRunLeft);
		m_updateFunc = &ActorPlayer::UpdateMoveLeft;
	}
}

/// <summary>
/// ���ˏ�ԂɑJ��
/// </summary>
void ActorPlayer::OnShot()
{
	ChangeAnim(kAnimIndexShot);
	m_updateFunc = &ActorPlayer::UpdateShot;
}

/// <summary>
/// �����[�h��ԂɑJ��
/// </summary>
void ActorPlayer::OnReload()
{
	ChangeAnim(kAnimIndexReload);
	m_updateFunc = &ActorPlayer::UpdateReload;
}
