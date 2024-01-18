#pragma once
#include <string>
#include <memory>
#include "Vec2.h"
#include "Utility/Collision.h"

struct size;
class FileBase;

/// <summary>
/// �{�X�̊��N���X
/// </summary>
class BossBase
{
public:
	BossBase(const size& windowSize, float fieldSize, int maxHp);
	virtual ~BossBase();

	virtual void Init(const Vec2& pos) = 0;
	void Update();
	void Draw() const;

	/// <summary>
	/// �����蔻��̒��S���W���擾
	/// </summary>
	/// <returns>�����蔻��̒��S���W</returns>
	Collision GetRect() const { return m_col; }

	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W�̒��S���W</returns>
	Vec2 GetPos() const { return m_pos; }

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

	/// <summary>
	/// HP��1���炷
	/// </summary>
	virtual bool OnAttack(bool isDash, const Collision& rect);

protected:
	/// <summary>
	/// �ǂɓ��������甽�˂�����
	/// </summary>
	virtual bool Reflection();
	/// <summary>
	/// ���˂�����v�Z
	/// </summary>
	void ReflectionCal(const Vec2& norVec);
	void ShiftReflection(const Vec2& shift);

	/// <summary>
	/// normal�֐��ɕύX����
	/// </summary>
	void ChangeNormalFunc();

	/// <summary>
	/// HitStop���J�n����
	/// </summary>
	void HitStop();


protected:
	// �����o�֐��|�C���^
	using updateFunc_t = void(BossBase::*)();
	using drawFunc_t = void(BossBase::*)() const;

	virtual void StartUpdate() = 0;
	virtual void NormalUpdate() = 0;
	void HitStopUpdate();

	virtual void StartDraw() const;
	virtual void NormalDraw() const;

	/// <summary>
	/// HP�o�[�̕`��
	/// </summary>
	void DrawHpBar() const;
	void DrawHitWallEffect() const;

protected:
	updateFunc_t m_updateFunc;
	drawFunc_t m_drawFunc;

	// ���̉�����܂ł̎���
	static const int kApeearFrame = 30;

	// �X�N���[���T�C�Y
	const size& m_size;
	// �t�B�[���h�T�C�Y
	const float m_fieldSize;

	// �L�����摜
	std::shared_ptr<FileBase> m_charImg;
	// �ǃG�t�F�N�g
	std::shared_ptr<FileBase> m_wallEffect;
	// �_���[�W�G�t�F�N�g
	std::shared_ptr<FileBase> m_damageEffect;

	// �G�̖��O
	std::string m_name;
	// �F
	unsigned int m_color;
	// �ő�̗�
	const int m_maxHp;
	// �̗�
	int m_hp;

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

	// �t���[��
	int m_frame;

	// HitStop�t���[��
	int m_hitStopFrame;

	// �ǂɓ��������t���[��
	int m_wallHitFrame;

	// �ǂɓ��������ۂ̏ꏊ
	int m_drawWallHitX;
	int m_drawWallHitY;

	// �_���[�W���󂯂��t���[��
	int m_onDamagetFrame;

	// �_���[�W���󂯂��ۂ̏ꏊ
	int m_drawOnDamagetX;
	int m_drawOnDamagetY;

	// �ǃG�t�F�N�g���
	int m_lineType;
};

