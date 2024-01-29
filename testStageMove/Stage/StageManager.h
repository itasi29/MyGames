#pragma once
#include <string>
#include <memory>
#include "../Vec2.h"

class StageBase;

class StageManager
{
public:
	StageManager();
	~StageManager();

	void Update();
	void Draw() const;

	void ChangeScene(const std::shared_ptr<StageBase> nextStage);
	void DeathMoveScreen(const Vec2& vec);

private:
	// �ʏ�̕`��
	void NormalUpdate();
	// �ړ��̕`��
	void StageMoveUpdate();
	void DeathMoveUpdate();

	// �ʏ�̕`��
	void NormalDraw() const;
	// �ړ����̕`��
	void StageMoveDraw() const;
	void DeathMoveDraw() const;

	Vec2 GetPos(const std::string& stage);

private:
	using UpdateFunc_t = void(StageManager::*)();
	using DrawFunc_t = void(StageManager::*)() const;

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	// ���݂̃X�e�[�W
	std::shared_ptr<StageBase> m_stage;
	// ���̃X�e�[�W
	std::shared_ptr<StageBase> m_nextStage;

	// �X�e�[�W�̏ꏊ
	Vec2 m_pos;
	// ���s���ꏊ
	Vec2 m_targetPos;
	// �ړ��p
	Vec2 m_vec;

	// ��ʕ`��p�n���h��
	int m_drawScreen;
	// �ړ����̉�ʃn���h��
	int m_screen;
	// �L�[�v�p��ʃn���h��
	int m_keepScreen;

	// �f�o�b�O�p
	Vec2 m_motoPos;

	// ���S���̉��o�p
	Vec2 m_deathVec;
	// �������̂�X�����
	bool m_isBaseX;
	int m_deathMoveFrame;
	double m_deathMoveAngle;
};

