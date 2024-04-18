#pragma once
#include <DxLib.h>

/// <summary>
/// �T�C�Y�\����
/// </summary>
struct Size
{
	// ��
	float w = 0.0f;
	// ����
	float h = 0.0f;
	// ���s��
	float d = 0.0f;
};

/// <summary>
/// �x�N�^�[�\����
/// </summary>
struct Vec3
{
	float x, y, z;

	Vec3();
	Vec3(float inX, float inY, float inZ);

	Vec3 operator+(const Vec3& val) const;
	void operator+=(const Vec3& val);
	Vec3 operator-(const Vec3& val) const;
	void operator-=(const Vec3& val);
	Vec3 operator*(float scale) const;
	void operator*=(float scale);
	Vec3 operator/(float scale) const;
	void operator/=(float scale);
	bool operator==(const Vec3& val) const;
	bool operator!=(const Vec3& val) const;

	/// <summary>
	/// DxLib�̊֐����g���ہAVECTOR�^�ɕϊ�����֐�
	/// </summary>
	/// <returns>VECTOR�^�̌��݂̒l</returns>
	VECTOR GetVECTOR() const { return VGet(x, y, z); }

	/// <summary>
	/// �������擾
	/// </summary>
	/// <returns>����</returns>
	float Length() const;
	/// <summary>
	/// �����̓����擾
	/// </summary>
	/// <returns>����</returns>
	float SqLength() const;
	/// <summary>
	/// ���g�̐��K��
	/// </summary>
	void Normalize();
	/// <summary>
	/// ���K���������ʂ��擾
	/// </summary>
	Vec3 GetNormalized() const;
};

/// <summary>
/// ���`���
/// </summary>
/// <param name="start">�X�^�[�g�n�_</param>
/// <param name="end">�I���n�_</param>
/// <param name="t">����</param>
/// <returns>��Ԍ�</returns>
Vec3 Lerp(const Vec3& start, const Vec3& end, float t);

using Pos3 = Vec3;

/// <summary>
/// ��`�̓����蔻��(2D����)
/// �������S
/// </summary>
struct Rect
{
private:
	// �ʒu
	Pos3 pos;
	// ������
	float h, w, d;

public:
	Rect();

	/// <summary>
	/// ��������ݒ�
	/// </summary>
	/// <param name="inH">����</param>
	/// <param name="inW">��</param>
	/// <param name="inD">���s��</param>
	void SetRange(float inH, float inW, float inD);
	/// <summary>
	/// �ꏊ��ݒ�
	/// </summary>
	/// <param name="inX">X���W</param>
	/// <param name="inY">Y���W</param>
	void SetPos(float inX, float inY);
	/// <summary>
	/// �ꏊ��ݒ�
	/// </summary>
	/// <param name="inPos">���W</param>
	void SetPos(const Pos3& inPos);
	/// <summary>
	/// �Փ˔���
	/// </summary>
	/// <param name="rect">���葤</param>
	/// <returns>true:�������� / false:�������Ă��Ȃ�</returns>
	bool IsHit(const Rect& rect);
	/// <summary>
	/// �����擾
	/// </summary>
	/// <returns>width</returns>
	float GetW() const { return w; }

	/// <summary>
	/// ����ꏊ�`��
	/// </summary>
	/// <param name="color">�J���[</param>
	void Draw(unsigned int color = 0xff0000) const;
};