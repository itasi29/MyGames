#pragma once
#include "Vec2.h"

/// <summary>
/// �~���Ǘ����邽�߂̃N���X
/// </summary>
class Collision
{
public:
	Collision();
	virtual ~Collision();

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
	/// <param name="shiftX">X�����ɒ��S����ǂꂭ�炢�����Ă��邩</param>
	/// <param name="shiftY">Y�����ɒ��S����ǂꂭ�炢�����Ă��邩</param>
	void SetCenter(Vec2 pos, float radius, float shiftX = 0.0f, float shiftY = 0.0f);

	// �K�v�ȏ����擾����
	float GetRadius() const { return m_radius; }
	Vec2 GetCenter() const { return m_pos; }

	/// <summary>
	/// ��`���m�̓����蔻��
	/// </summary>
	/// <param name="target">���葤</param>
	/// <returns>true : ��������, false : �������ĂȂ�</returns>
	bool IsCollsion(const Collision& target) const;

private:
	// ���S���W
	Vec2 m_pos;
	// ���a
	float m_radius;
};

