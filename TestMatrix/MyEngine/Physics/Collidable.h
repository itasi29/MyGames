#pragma once
#include <memory>
#include "../ObjectTag.h"
#include "Rigidbody.h"

namespace MyEngine
{
	class Physics;
	class ColliderBase;

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
		virtual void OnCollide(const Collidable& colider) abstract;

		/* Getter */
		ObjectTag GetTag() const { return m_tag; }
		Priority GetPriority() const { return m_priority; }

	private:
		std::shared_ptr<ColliderBase> CreateColliderData(const ColliderBase::Kind& kind);

	protected:
		// �����f�[�^
		Rigidbody m_rigid;
		// �����蔻��f�[�^
		std::shared_ptr<ColliderBase> m_collider;

	private:
		ObjectTag m_tag;
		Priority m_priority;
	};
}

