#pragma once
#include <DxLib.h>
#include <string>
#include <memory>
#include <list>
#include "Vec2.h"
#include "Utility/Collision.h"

struct size;
class FileBase;

struct PerformanceEff
{
	Vec2 pos;
	int frame = 0;
	bool isEnd = false;
};

/// <summary>
/// �{�X�̊��N���X
/// </summary>
class BossBase
{
public:
	BossBase(const size& windowSize, float fieldSize, int maxHp);
	virtual ~BossBase();

	void TitleInit();
	void TitleUpdate();
	void TitleDraw();
	virtual void Init(const Vec2& pos, bool isStart = false) = 0;
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
	/// ���S���o���I�����Ă��邩
	/// </summary>
	/// <returns>true : �I�� / false : ���o��</returns>
	bool IsEndPerformance() const { return m_isEndPerformance; }

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


	/// <summary>
	/// ���S���̏���
	/// </summary>
	void OnDeath();

protected:
	// �����o�֐��|�C���^
	using updateFunc_t = void(BossBase::*)();
	using drawFunc_t = void(BossBase::*)() const;

	virtual void StartUpdate() = 0;
	virtual void NormalUpdate() = 0;
	void HitStopUpdate();
	void DeathUpdate();

	/*���S���o���̍X�V�֐�*/
	void ExplotionUpdate();
	void ShakeUpdate();
	void LastUpdate();

	virtual void StartDraw() const;
	virtual void NormalDraw() const;
	void DeathDraw() const;

	/*���S���o���̕`��֐�*/
	void ExplotionDraw() const;
	void ShakeDraw() const;
	void LastDraw() const;

	/// <summary>
	/// HP�o�[�̕`��
	/// </summary>
	void DrawHpBar() const;
	void DrawHitWallEffect() const;
	void DrawDamageEffect() const;

protected:
	// HP�o�[�̕�
	const int kHpBarWidth = 270;
	// �p�x
	const double kRad = DX_PI / 180;

	// �_���[�W���󂯂��ۂ̃t���[��
	const int kOnDamageFrame = 33;

	updateFunc_t m_updateFunc;
	drawFunc_t m_drawFunc;
	updateFunc_t m_deathUpdateFunc;
	drawFunc_t m_deathDrawFunc;

	// ���̉�����܂ł̎���
	static const int kApeearFrame = 30;

	double m_angle;

	// �X�N���[���T�C�Y
	const size& m_size;
	// �t�B�[���h�T�C�Y
	const float m_fieldSize;

	// �L�����摜
	std::shared_ptr<FileBase> m_charImg;
	std::shared_ptr<FileBase> m_shadow;
	// �ǃG�t�F�N�g
	std::shared_ptr<FileBase> m_wallEffect;
	// �_���[�W�G�t�F�N�g
	std::shared_ptr<FileBase> m_damageEffect;
	// HP�o�[�֘A�̉摜
	std::shared_ptr<FileBase> m_hpBar;
	std::shared_ptr<FileBase> m_hpBarBack;
	std::shared_ptr<FileBase> m_hpBarDown;
	std::shared_ptr<FileBase> m_hpBarFrame;

	// ��������SE
	std::shared_ptr<FileBase> m_createSe;
	// �_���[�W���󂯂�����SE
	std::shared_ptr<FileBase> m_damageSe;

	// �G�̖��O
	std::string m_name;
	// �F
	unsigned int m_color;
	// �ő�̗�
	const int m_maxHp;
	// �̗�
	int m_hp;
	// �ʏ�̃o�[�̕�
	int m_hpWidth;
	// �Ԃ��o�[�̕�
	int m_hpDownWidth;

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

	// ���S���o�t���O
	bool m_isEndPerformance;
	// ���o�t���[��
	int m_endPerformanceFrame;
	// ���o��
	int m_performanceNum;
	// �k�킹�t���O
	bool m_isShake;
	// ���U�t���O
	bool m_isScatter;
	// �����G�t�F�N�g�摜
	std::shared_ptr<FileBase> m_performance;
	// �G�t�F�N�g�`��ʒu���
	std::list<PerformanceEff> m_performanceEff;
	// �g��̔��a
	int m_ripple1;
	int m_ripple2;
	int m_ripple3;
	// ���a�̐F���c���p
	int m_rippleScreen;
};

