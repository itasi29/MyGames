#pragma once
#include "ColliderBase.h"

namespace MyEngine
{
	class ColliderSphere : public ColliderBase
	{
	public:
		ColliderSphere();

	public:
		// MEMO: �F�X�ȂƂ���Ŏg�����߃R�[�f�B���O�K�񖳎����Ă���
		float radius;
	};
}

