#pragma once
#include "../Utility/Vec2.h"
#include "../Utility/Rect.h"

class Input;
class Application;
struct Size;

class Player
{
public:
	Player(Application& app);
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
	// �A�v���P�[�V�����N���X
	Application& m_app;
	// Windows�̃T�C�Y
	const Size& m_size;

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
	// ���`��ԗp
	Vec2 m_interpolatedValue;
	int m_interpolatedFrame;

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
	/// ���`���
	/// </summary>
	void Lerp();

	void Complex();
};

