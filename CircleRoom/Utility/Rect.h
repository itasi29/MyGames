#pragma once
#include "Vec2.h"

/// <summary>
/// �~���Ǘ����邽�߂̃N���X
/// </summary>
class Rect
{
public:
	Rect();
	virtual ~Rect();

	/// <summary>
	/// �����蔻��̕`��
	/// </summary>
	/// <param name="color">�F</param>
	/// <param name="isFill">true:�S�̕`�� / false:�O�g�̂ݕ`��</param>
	void Draw(unsigned int color, bool isFill) const;
	
	/// <summary>
	/// ���S�n�̍X�V
	/// </summary>
	/// <param name="pos">���S�n</param>
	/// <param name="radius">���a</param>
	void SetCenter(Vec2 pos, float radius);

	// �K�v�ȏ����擾����
	float GetRadius() const;
	Vec2 GetCenter() const;		// ��`�̒��S���W

	/// <summary>
	/// ��`���m�̓����蔻��
	/// </summary>
	/// <param name="target">���葤</param>
	/// <returns>true : ��������, false : �������ĂȂ�</returns>
	bool IsCollsion(const Rect& target);

private:
	// ���S���W
	Vec2 m_pos;
	// ���a
	float m_radius;
};

