#pragma once
#include <memory>
#include <vector>
#include "Common/ObjectTag.h"
#include "Rigidbody.h"
#include "ColliderBase.h"

namespace MyEngine
{
	class Physics;
//	class ColliderBase;

	/// <summary>
	/// 衝突できるもの
	/// </summary>
	class Collidable abstract
	{
		friend Physics;
	public:
		// 優先度判別
		enum class Priority : int
		{
			Low,		// 低
			Middle,		// 中
			High,		// 高
			Static,		// 動かない（最高）
		};

	public:
		Collidable(Priority priority, ObjectTag tag, const ColliderBase::Kind& kind);
		virtual ~Collidable();

		// 衝突したとき
		virtual void OnCollideEnter(const Collidable& colider) {}
		virtual void OnCollideStay(const Collidable& colider) {}
		virtual void OnCollideExit(const Collidable& colider) {}
		virtual void OnTriggerEnter(const Collidable& colider) {}
		virtual void OnTriggerStay(const Collidable& colider) {}
		virtual void OnTriggerExit(const Collidable& colider) {}

		/* Getter */
		ObjectTag GetTag() const { return m_tag; }
		Priority GetPriority() const { return m_priority; }

	protected:
		void AddCollider(const ColliderBase::Kind& kind);

	private:
		void CreateColliderData(const ColliderBase::Kind& kind);

	protected:
		// 物理データ
		Rigidbody m_rigid;
		// 当たり判定データ
		std::vector<std::shared_ptr<ColliderBase>> m_collider;

	private:
		ObjectTag m_tag;
		Priority m_priority;
	};
}

