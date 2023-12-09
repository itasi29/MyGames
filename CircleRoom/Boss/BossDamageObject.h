#pragma once
#include "Vec2.h"
#include "Collision.h"

struct Size;

/// <summary>
/// �{�X�Ƀ_���[�W��^���镨��
/// </summary>
class BossDamageObject
{
public:
	BossDamageObject(const Size& windowSize, float fieldSize);
	BossDamageObject(const Vec2& col);
	~BossDamageObject();

	void Draw();

	/// <summary>
	/// �����蔻��̒��S���W���擾
	/// </summary>
	/// <returns>�����蔻��̒��S���W</returns>
	Collision GetRect() const { return m_col; }

	/// <summary>
	/// �g�������Ƃɂ���
	/// </summary>
	void Used() { m_isUsed = true; }

	/// <summary>
	/// ���Ɏg�p�ς݂ɂȂ��Ă��Ȃ���
	/// </summary>
	/// <returns>true: �g�p�ς� / false:���g�p</returns>
	bool IsUsed() const { return m_isUsed; }

private:
	Vec2 m_pos;
	Collision m_col;

	bool m_isUsed;
};

