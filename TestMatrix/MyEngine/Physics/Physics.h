#pragma once
#include <memory>
#include <list>
#include <functional>
#include "Common/ObjectTag.h"

namespace MyEngine
{
	class Collidable;
	class ColliderBase;

	class Physics final
	{
	private:
		enum class OnCollideInfoKind
		{
			CollideEnter,
			CollideStay,
			CollideExit,
			TriggerEnter,
			TriggerStay,
			TriggerExit
		};
		struct OnCollideInfo
		{
			Collidable* send;
			OnCollideInfoKind kind;
			std::function<void(const Collidable&)> func;
		};
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

		bool IsCollide(const Collidable* objA, const Collidable* objB, const std::shared_ptr<ColliderBase>& colliderA, const std::shared_ptr<ColliderBase>& colliderB) const;
		void FixNextPos(const Collidable* primary, Collidable* secondary, const std::shared_ptr<ColliderBase>& colliderPrimary, const std::shared_ptr<ColliderBase>& colliderSecondary) const;
		void AddOnCollideInfo(Collidable* objA, Collidable* objB, OnCollideInfoKind kind);
		std::function<void(const Collidable&)> GetOnCollideInfoFunc(Collidable* obj, OnCollideInfoKind kind);
		void FixPos() const;

	private:
		std::list<std::shared_ptr<Collidable>> m_collidables;
		std::list<OnCollideInfo> m_onCollideInfo;
	};
}

