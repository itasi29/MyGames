#pragma once

namespace MyEngine
{
	class ColliderBase abstract
	{
	public:
		// 当たり判定種別
		enum class Kind
		{
			Sphere,
		};

		// コンストラクタ
		ColliderBase(Kind kind) : m_kind(kind) {}
		virtual ~ColliderBase() {}

		// 当たり判定種別取得
		Kind GetKind() const { return m_kind; }

	private:
		Kind m_kind;
	};
}
