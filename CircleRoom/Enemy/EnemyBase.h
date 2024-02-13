#pragma once
#include <DxLib.h>
#include <string>
#include <memory>
#include <vector>
#include <list>
#include "Vec2.h"
#include "Collision.h"

struct size;
class FileBase;

struct WallEff
{
	Vec2 pos;
	Vec2 vec;

	double size = 0.0;
};

struct WallEffMass
{
	std::vector<WallEff> effs;
	bool isUse = true;
	int frame = 0;
};

/// <summary>
/// Enemy�N���X�̊��
/// </summary>
class EnemyBase
{
public:
	EnemyBase(const size& windowSize, float fieldSize);
	virtual ~EnemyBase();

	void TitleInit();
	void TitleUpdate();
	void TitleDraw() const;
	virtual void Init(const Vec2&, bool isStart = false) {}
	void Update();
	void Draw() const;

	/// <summary>
	/// �����蔻��̒��S���W���擾
	/// </summary>
	/// <returns>�����蔻��̒��S���W</returns>
	Collision GetRect() const { return m_col; }

	/// <summary>
	/// �G�̖��O��Ԃ�
	/// </summary>
	/// <returns>���O</returns>
	std::string GetName() const { return m_name; }

	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>true: �����Ă� / false:����ł�</returns>
	bool IsExsit() const { return m_isExsit; }

protected:
	/// <summary>
	/// �ǂɓ��������甽�˂�����
	/// </summary>
	/// <param name="scale">���a�̑傫��</param>
	/// <param name="isShift">�ǂɓ����������ɔ��˂����邩</param>
	virtual bool Reflection(float scale = 0.0f, bool isShift = true);
	/// <summary>
	/// ���˂�����v�Z
	/// </summary>
	/// /// <param name="norVec">�@���x�N�g��</param>
	void ReflectionCal(const Vec2& norVec);
	/// <summary>
	/// ���ˎ��ɂ��炷
	/// </summary>
	/// <param name="shift">���炷����</param>
	void ShiftReflection(const Vec2& shift);

	/// <summary>
	/// normal�֐��ɕύX����
	/// </summary>
	void ChangeNormalFunc();

protected:
	// �X�V�֐�
	virtual void StartUpdate() = 0;
	virtual void NormalUpdate() = 0;

	// �`��֐�
	virtual void StartDraw() const;
	virtual void NormalDraw() const;

	/// <summary>
	/// �ǂɓ����������̃G�t�F�N�g�`��
	/// </summary>
	void DrawHitWallEffect() const;

private:
	/// <summary>
	/// �ǂɓ����������̃G�t�F�N�g�̒ǉ�
	/// </summary>
	/// <param name="pos">�ꏊ</param>
	/// <param name="sizeX">�����T�C�YX : 16�������ǂ̕����܂őΉ������邩</param>
	/// <param name="shiftX">���炵�xX</param>
	/// <param name="sizeY">�����T�C�YY</param>
	/// <param name="shiftY">���炵�xY</param>
	void AddWallEff(const Vec2& pos, int sizeX, float shiftX, int sizeY, float shiftY);

protected:
	// 1�x���ʓx�@��
	const double kRad = DX_PI / 180;

	// �����o�֐��|�C���^
	using updateFunc_t = void(EnemyBase::*)();
	using drawFunc_t = void(EnemyBase::*)() const;

	updateFunc_t m_updateFunc;
	drawFunc_t m_drawFunc;

	// ���̉�����܂ł̎���
	static const int kApeearFrame = 60;
	// �ړ��G�t�F�N�g�̃t���[��
	static const int kEffFrame = 10;

	// �X�N���[���T�C�Y
	const size& m_size;
	// �t�B�[���h�̃T�C�Y
	const float m_fieldSize;

	// �摜
	std::shared_ptr<FileBase> m_charImg;
	std::shared_ptr<FileBase> m_shadow;
	std::shared_ptr<FileBase> m_wallEffect;

	// ��������SE
	std::shared_ptr<FileBase> m_createSe;

	// �G�̖��O
	std::string m_name;

	// ���S���W
	Vec2 m_pos;
	// �ړ��x�N�g��
	Vec2 m_vec;
	// �����蔻��
	Collision m_col;
	// ���a
	float m_radius;

	// ��������
	bool m_isExsit;

	// �摜�̉�]
	double m_angle;

	// �t���[��
	int m_frame;

	// �ǂɓ����������̃G�t�F�N�g
	std::list<WallEffMass> m_wallEff;
};

