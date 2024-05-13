#pragma once
#include <DxLib.h>

class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void Update();

private:
	VECTOR m_pos;


	float m_angle = -DX_PI_F / 2;
};

