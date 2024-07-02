#pragma once
#include "ColliderBase.h"

namespace MyEngine
{
	class ColliderSphere : public ColliderBase
	{
	public:
		ColliderSphere();

	public:
		// MEMO: 色々なところで使うためコーディング規約無視している
		float radius;
	};
}

