#pragma once
#include "../Utility/Vec2.h"
#include "../Utility/Rect.h"

class Input;
class Application;
struct Size;

class Player
{
public:
	Player(const Size& windowSize, float fieldSize);
	~Player();

	void Update(Input& input);
	void Draw();

	/// <summary>
	/// ���S���W���擾
	/// </summary>
	/// <returns>���S���W</returns>
	Vec2 GetPos() const { return m_pos; }

	/// <summary>
	/// �����蔻��̒��S���W���擾
	/// </summary>
	/// <returns>�����蔻��̒��S���W</returns>
	Rect GetColPos() const { return m_colRect; }
	/// <summary>
	/// �����蔻��̔��a���擾
	/// </summary>
	/// <returns>�����蔻��̔��a</returns>
	float GetColRadius() const { return m_colRaidus; }

private:
	// �X�N���[���T�C�Y
	const Size& m_windowSize;
	// �t�B�[���h�̃T�C�Y
	float m_fieldSize;

	// ���S���W
	Vec2 m_pos;
	// ���ʃx�N�g��
	Vec2 m_frontVec;
	// �E�x�N�g��
	Vec2 m_rightVec;
	// ���x�N�g��
	Vec2 m_leftVec;
	// ���݂̐��ʕ���
	Vec2 m_nowFront;

	// �����蔻��
	Rect m_colPos;

	// �ړ��x�N�g��
	Vec2 m_vec;
	// �_�b�V������t���[��
	int m_dashFrame;
	// �_�b�V���ė��p�\�܂ł̃t���[��
	int m_dashWaitFrame;
	// �_�b�V������
	bool m_isDash;

	// �����蔻��
	Rect m_colRect;
	// �����蔻��̔��a
	float m_colRaidus;

	// �������Ă��邩
	bool m_isExsit;

private:
	/// <summary>
	/// �v���C���[�̈ړ�
	/// </summary>
	/// <param name="input">���͏��</param>
	void Move(Input& input);
	/// <summary>
	/// �_�b�V������
	/// </summary>
	/// <param name="input">���͏��</param>
	void Dash(Input& input);
	/// <summary>
	/// ��O���菈��
	/// </summary>
	void InRange();
};

