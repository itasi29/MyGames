#pragma once
#include <vector>
#include <list>
#include "Vec2.h"
#include "Utility/Collision.h"

class Input;
class Application;
class FileBase;
struct size;
enum class Ability;

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
	Player(const size& windowSize, float fieldSize);
	~Player();

	/// <summary>
	/// ����������
	/// </summary>
	void Init();
	void Update(Input& input, Ability ability);
	void Draw() const;

	/// <summary>
	/// ���ʃx�N�g�����擾
	/// </summary>
	/// <returns>���ʃx�N�g��</returns>
	Vec2 GetFront() const { return m_front; }

	/// <summary>
	/// ���S���W���擾
	/// </summary>
	/// <returns>���S���W</returns>
	Vec2 GetPos() const { return m_pos; }

	/// <summary>
	/// �����蔻��̒��S���W���擾
	/// </summary>
	/// <returns>�����蔻��̒��S���W</returns>
	Collision GetRect() const { return m_col; }
	/// <summary>
	/// �I�u�W�F�N�g�̓����蔻����擾
	/// </summary>
	/// <returns>�I�u�W�F�N�g����̒��S���W</returns>
	Collision GetObjRect() const { return m_objCol; }
	bool IsDash() const { return m_isDash; }
	/// <summary>
	/// �������Ă��邩
	/// </summary>
	/// <returns>��������</returns>
	bool IsExsit() const { return m_isExsit; }

	/// <summary>
	/// ���S����
	/// </summary>
	void Death();

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
	/// <summary>
	/// �ړ��G�t�F�N�g�̒ǉ�
	/// </summary>
	void AddMoveEff();

private:
	std::shared_ptr<FileBase> m_bloodImg;

	// �X�N���[���T�C�Y
	const size& m_size;
	// �t�B�[���h�̃T�C�Y
	float m_fieldSize;

	// �摜
	std::shared_ptr<FileBase> m_charImg;
	std::shared_ptr<FileBase> m_charDeathImg;
	std::shared_ptr<FileBase> m_charEffImg;

	// ���S��Se
	std::shared_ptr<FileBase> m_deathSe;
	// �_�b�V����Se
	std::shared_ptr<FileBase> m_dashSe;

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

	// �����蔻��
	Collision m_col;
	// �_���[�W�I�u�W�F�N�g�����蔻��
	Collision m_objCol;

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

	// �������Ă��邩
	bool m_isExsit;

	// ���S�G�t�F�N�g�̃t���[��
	int m_deathFrame;
	// ���S�G�t�F�N���s�����ǂ���
	bool m_isDeathEffect;
};

