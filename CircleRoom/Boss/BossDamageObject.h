#pragma once
#include <vector>
#include <memory>
#include "Vec2.h"
#include "Collision.h"

struct size;
class BossBase;
class FileBase;

// �g�p��̃{�X�ɔ��ł������
struct Missile
{
	// �ꏊ
	Vec2 pos;
	// �����蔻��
	Collision col;
	// �x�N�g��
	Vec2 vec;
	// �{�X�ɓ���������
	bool isHit = false;
};

struct MissileEff
{
	Vec2 pos;
	Vec2 vec;

	double size;
};

struct MissileEffMass
{
	std::vector<MissileEff> effs;
	bool isUse = false;
	bool isEnd = false;
	int frame = 0;
};

/// <summary>
/// �{�X�Ƀ_���[�W��^���镨��
/// </summary>
class BossDamageObject
{
public:
	BossDamageObject(const size& windowSize, float fieldSize, BossBase* boss);
	BossDamageObject(const Vec2& col, BossBase* boss);
	~BossDamageObject();

	void Update();
	void Draw();

	/// <summary>
	/// �����蔻��̒��S���W���擾
	/// </summary>
	/// <returns>�����蔻��̒��S���W</returns>
	Collision GetRect() const { return m_col; }

	/// <summary>
	/// �U�����悤
	/// </summary>
	void OnUse();

	/// <summary>
	/// ���Ɏg�p�ς݂ɂȂ��Ă��Ȃ���
	/// </summary>
	/// <returns>true: �g�p�ς� / false:���g�p</returns>
	bool IsEnd() const { return m_isUsed; }

	bool IsPickUp() const { return m_isPickUp; }

private:
	// �E����܂ł̃A�b�v�f�[�g
	void FlashUpdate();
	// �E�������Ƃ̃A�b�v�f�[�g
	void AimUpdae();

	void FlashDraw();
	void AimDraw();

	void Move(Missile& missile);

private:
	using UpdateFunc_t = void(BossDamageObject::*)();
	using DrawFunc_t = void(BossDamageObject::*)();

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	std::shared_ptr<FileBase> m_missileEff;

	// �{�X�̃|�C���^
	BossBase* m_boss;

	Vec2 m_pos;
	Collision m_col;

	bool m_isPickUp;
	bool m_isUsed;
	int m_flashFrame;

	std::vector<Missile> m_missiles;
	std::vector<MissileEffMass> m_missileEffs;
};

