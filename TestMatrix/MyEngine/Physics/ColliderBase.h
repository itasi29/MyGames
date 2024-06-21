#pragma once

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
		ColliderBase(Kind kind) : m_kind(kind) {}
		virtual ~ColliderBase() {}

		// �����蔻���ʎ擾
		Kind GetKind() const { return m_kind; }

	private:
		Kind m_kind;
	};
}
