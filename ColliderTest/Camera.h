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
};

