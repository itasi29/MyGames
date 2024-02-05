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

	double size;
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
	void ReflectionCal(const Vec2& norVec);
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

	void DrawHitWallEffect() const;

private:
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

	// �L�����摜
	std::shared_ptr<FileBase> m_charImg;
	std::shared_ptr<FileBase> m_shadow;
	// �ǃG�t�F�N�g
	std::shared_ptr<FileBase> m_wallEffect;

	// ��������SE
	std::shared_ptr<FileBase> m_createSe;

	// �G�̖��O
	std::string m_name;
	// �G�̃J���[(�O���t�ɕς������������)
	unsigned int m_color;

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

	double m_angle;

	// �t���[��
	int m_frame;

	std::list<WallEffMass> m_wallEff;
};

