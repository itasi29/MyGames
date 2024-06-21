#pragma once
#include <memory>
#include <list>

namespace MyEngine
{
	class Collidable;

	class Physics final
	{
	private:
		Physics();

		Physics(const Physics&) = delete;
		void operator= (const Physics&) = delete;
	public:
		static Physics& GetInstance();

		void Entry(const std::shared_ptr<Collidable>& collidable);
		void Exit(const std::shared_ptr<Collidable>& collidable);

		void Update();

	private:
		void MoveNextPos() const;

		void CheckCollide();

		bool IsCollide(const std::shared_ptr<Collidable>& objA, const std::shared_ptr<Collidable>& objB) const;
		void FixNextPos(const std::shared_ptr<Collidable>& primary, const std::shared_ptr<Collidable>& secondary) const;
		void AddOnCollideInfo(const std::shared_ptr<Collidable>& objA, const std::shared_ptr<Collidable>& objB);

		void FixPos() const;

	private:
		std::list<std::shared_ptr<Collidable>> m_collidables;
	};
}

