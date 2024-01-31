#pragma once
#include <vector>
#include <list>
#include "Vec2.h"

class Input;

struct DirectionVec
{
	Vec2 front;
	Vec2 right;
	Vec2 left;
};

struct EffectData
{
	Vec2 vec;
	Vec2 pos;
	double angle = 0.0;
	int frame = 0;
	bool isEnd = false;
};


class Player
{
public:
	Player();
	~Player();

	/// <summary>
	/// ����������
	/// </summary>
	void Init();
	void Update(Input& input);
	void Draw();

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

private:
	// ���S���W
	Vec2 m_pos;
	// ���ʕ���
	Vec2 m_front;
	// �ړ��x�N�g��
	Vec2 m_vec;
	// �����x�N�g��
	DirectionVec m_dir;

	// �ړ��G�t�F�N�g
	std::list<EffectData> m_effs;

	// �p�x
	double m_angle;

	int handle;

	// �_�b�V�����O
	std::vector<Vec2> m_posLog;
	std::vector<DirectionVec> m_dirLog;
	std::vector<double> m_angleLog;
	// ���O�t���[��
	int m_logFrame;
	// �_�b�V������t���[��
	int m_dashFrame;
	// �_�b�V���ė��p�\�܂ł̃t���[��
	int m_dashWaitFrame;
	// �_�b�V������
	bool m_isDash;
};

