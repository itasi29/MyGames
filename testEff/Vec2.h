#pragma once
#include <cmath>

/// �x�N�g����֗��Ɏg�����߂̃N���X(2�����x�N�g��)
class Vec2
{
public:
	float x;
	float y;

public:
	Vec2() :
		x(0.0f),
		y(0.0f)
	{
	}

	Vec2(float posX, float posY) :
		x(posX),
		y(posY)
	{
	}

	static Vec2 Zero()
	{
		return Vec2{ 0, 0 };
	}
	static Vec2 Up()
	{
		return Vec2{ 0.0f, -1.0f };
	}
	// ���g����-90�x�����x�N�g��
	Vec2 Right() const
	{
		return Vec2{ y, -x };
	}
	// ���g����90�x�����x�N�g��
	Vec2 Left() const
	{
		return Vec2{ -y, x };
	}

	/// �P�����Z�q+
	Vec2 operator +() const
	{
		// this�|�C���^ : �������g�̃A�h���X���w��
		return *this;
	}

	/// �P�����Z�q- �t�x�N�g��
	Vec2 operator -() const
	{
		return Vec2{ -x, -y };
	}

	/// �����Z
	Vec2 operator +(Vec2 vec) const
	{
		return Vec2{ x + vec.x, y + vec.y };
	}
	Vec2 operator +=(Vec2 vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	/// �����Z
	Vec2 operator -(Vec2 vec) const
	{
		return Vec2{ x - vec.x, y - vec.y };
	}
	Vec2 operator -=(Vec2 vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	/// �|���Z
	Vec2 operator *(float scale) const
	{
		return Vec2{ x * scale, y * scale };
	}
	Vec2 operator *=(float scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}

	/// ����Z
	Vec2 operator /(float scale) const
	{
		return Vec2{ x / scale, y / scale };
	}
	Vec2 operator /=(float scale)
	{
		x /= scale;
		y /= scale;
		return *this;
	}

	Vec2 operator *(Vec2 left) const
	{
		Vec2 temp;

		temp.x = x * left.x - y * left.y;
		temp.y = x * left.y + y * left.x;

		return temp;
	}
	Vec2 operator *=(Vec2 left)
	{
		Vec2 temp = this->GetNormalized();

		x = temp.x * left.x - temp.y * left.y;
		y = temp.x * left.y + temp.y * left.x;

		return *this;		
	}

	bool operator ==(Vec2 left)
	{
		return x == left.x && y == left.y;
	}
	bool operator !=(Vec2 left)
	{
		return !(*this == left);
	}

	/// <summary>
	/// �x�N�g���̒�����2������߂�
	/// </summary>
	/// <returns>������2��</returns>
	float SqLength() const
	{
		return x * x + y * y;
	}

	/// <summary>
	/// �x�N�g���̒��������߂�
	/// </summary>
	/// <returns>����</returns>
	float Length() const
	{
		return sqrtf(SqLength());
	}

	/// <summary>
	/// ���g�̐��K�����s��
	/// </summary>
	void Normalize()
	{
		float len = Length();

		// ����0�ȉ��Ȃ珈�����s��Ȃ�
		if (len <= 0.0f) return;

		x /= len;
		y /= len;
	}

	/// <summary>
	/// ���g�𐳋K�������x�N�g�����擾����
	/// </summary>
	/// <returns>���K�������x�N�g��</returns>
	Vec2 GetNormalized() const
	{
		float len = Length();

		// ����0�ȉ��Ȃ�(0, 0)��Ԃ�
		/* ��������ɂ��Ă��闝�R */
		// �����Ȃ��R���X�g���N�^�̏C���ɋ������邽��
		if (len <= 0.0f) return Vec2{ 0.0f, 0.0f };

		return Vec2{ x / len, y / len };
	}

	/// <summary>
	/// ���ς��擾����
	/// </summary>
	/// <param name="vec2">���߂鑊��̃x�N�g��</param>
	/// <returns>����</returns>
	float Dot(Vec2 vec2) const
	{
		return x * vec2.x + y * vec2.y;
	}
};