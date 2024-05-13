#include "Camera.h"
#include <cmath>

namespace
{
	constexpr float kDist = 96;
	constexpr float kHeight = 32;

	constexpr float kNear = 1.0f;
	constexpr float kFar = 180.0f;
}

Camera::Camera() :
	m_pos{},
	m_angle(0.0f)
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
	m_angle = -DX_PI_F / 2;
}

void Camera::Update()
{
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_LEFT))
	{
		m_angle += 0.05f;
	}
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_RIGHT))
	{
		m_angle -= 0.05f;
	}


	SetCameraNearFar(kNear, kFar);
	VECTOR cameraPos;
	cameraPos.x = cosf(m_angle) * kDist;
	cameraPos.y = kHeight;
	cameraPos.z = sinf(m_angle) * kDist;
	SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));
}
