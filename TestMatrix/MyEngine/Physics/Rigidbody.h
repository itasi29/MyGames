#pragma once
#include "../Geometry/Vec3.h"

namespace MyEngine
{
	class Rigidbody final
	{
	public:
		Rigidbody();
		~Rigidbody();

		void Init(const Vec3& pos = Vec3());

		/* Getter/Setter */
		const Vec3& GetPos() const { return m_pos; }
		const Vec3& GetNextPos() const { return m_nextPos; }
		const Vec3& GetVelocity() const { return m_velocity; }
		const Vec3& GetDir() const { return m_dir; }

		void SetPos(const Vec3& pos) { m_pos = pos; }
		void SetNextPos(const Vec3& nextPos) { m_nextPos = nextPos; }
		void SetVelocity(const Vec3& velocity);

	private:
		Vec3 m_pos;
		Vec3 m_nextPos;
		Vec3 m_velocity;
		Vec3 m_dir;
	};
}

