#pragma once
#include <unordered_map>

namespace MyEngine
{
	class ColliderBase abstract
	{
	public:
		// �����蔻����
		enum class Kind
		{
			Sphere,
		};

		// �R���X�g���N�^
		ColliderBase(Kind kind);
		virtual ~ColliderBase() {}

		// �����蔻���ʎ擾
		Kind GetKind() const { return m_kind; }
		// �����������X�V
		void UpdateHit(const ColliderBase* collider, bool isHit);
		bool IsHit(const ColliderBase* collider) const { return m_isHit.at(collider); }
		bool IsPreHit(const ColliderBase* collider) const { return m_isPreHit.at(collider); }

	private:
		Kind m_kind;
		std::unordered_map<const ColliderBase*, bool> m_isHit;
		std::unordered_map<const ColliderBase*, bool> m_isPreHit;
	};
}
