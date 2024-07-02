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
		struct OnCollideInfoData
		{
			Collidable* own;
			Collidable* send;
			OnCollideInfoKind kind;
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

		bool IsCollide(const std::shared_ptr<Collidable>& objA, const std::shared_ptr<Collidable>& objB) const;
		void FixNextPos(const std::shared_ptr<Collidable>& primary, std::shared_ptr<Collidable>& secondary) const;
#if false
		void AddOnCollideInfo(Collidable* objA, Collidable* objB, OnCollideInfoKind kind);
#else
		void AddOnCollideInfo();
#endif
		void OnCollideInfo(Collidable* own, Collidable* send, OnCollideInfoKind kind);
		void FixPos() const;

	private:
		std::list<std::shared_ptr<Collidable>> m_collidables;
		std::list<OnCollideInfoData> m_onCollideInfo;
		std::unordered_map<Collidable*, std::list<Collidable*>> m_newCollideInfo;
		std::unordered_map<Collidable*, std::list<Collidable*>> m_preCollideInfo;
	};
}

