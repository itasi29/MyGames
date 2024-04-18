#include "ActorBase.h"
#include <DxLib.h>
#include <cassert>
#include "ActorManager.h"

namespace
{
	constexpr float kAnimSpeed = 1.0f;
}

ActorBase::ActorBase(int handle, ActorManager& actorMgr, FileManager& fileMgr, SoundSystem& sndSys, const wchar_t* const name) :
	m_attachIndex(0),
	m_nowAnimTime(0.0f),
	m_totalAnimTime(0.0f),
	m_isAnimOneLoop(false),
	m_sndSys(sndSys),
	m_isExist(true)
{
	m_handle = MV1DuplicateModel(handle);
	assert(m_handle > 0);

	const auto& scale = actorMgr.GetActorData(name).model;
	MV1SetScale(m_handle, VGet(scale.w, scale.h, scale.d));
}

ActorBase::~ActorBase()
{
	MV1DeleteModel(m_handle);
}

void ActorBase::Init(float x, float y)
{
	m_pos.x = x;
	m_pos.y = y;
}

void ActorBase::Update(Input& input)
{
	// 座標・判定更新
	m_pos += m_vel;
	MV1SetPosition(m_handle, m_pos.GetVECTOR());
	m_rect.SetPos(m_pos);

	// アニメーション更新
	MV1SetAttachAnimTime(m_handle, m_attachIndex, m_nowAnimTime);
	m_nowAnimTime += kAnimSpeed;
	if (m_nowAnimTime >= m_totalAnimTime) {
		m_nowAnimTime = 0.0f;
		m_isAnimOneLoop = true;
	}
}

void ActorBase::Draw() const
{
	MV1DrawModel(m_handle);
#ifdef _DEBUG
	m_rect.Draw();
#endif
}

void ActorBase::ChangeAnim(int animIndex)
{
	MV1DetachAnim(m_handle, m_attachIndex);
	m_attachIndex = MV1AttachAnim(m_handle, animIndex);
	m_nowAnimTime = 0.0f;
	m_totalAnimTime = MV1GetAttachAnimTotalTime(m_handle, m_attachIndex);
	m_isAnimOneLoop = false;
}
