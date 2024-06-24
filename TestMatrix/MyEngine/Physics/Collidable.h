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
	/// �Փ˂ł������
	/// </summary>
	class Collidable abstract
	{
		friend Physics;
	public:
		// �D��x����
		enum class Priority : int
		{
			Low,		// ��
			Middle,		// ��
			High,		// ��
			Static,		// �����Ȃ��i�ō��j
		};

	public:
		Collidable(Priority priority, ObjectTag tag, const ColliderBase::Kind& kind);
		virtual ~Collidable();

		// �Փ˂����Ƃ�
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
		// �����f�[�^
		Rigidbody m_rigid;
		// �����蔻��f�[�^
		std::vector<std::shared_ptr<ColliderBase>> m_collider;

	private:
		ObjectTag m_tag;
		Priority m_priority;
	};
}

