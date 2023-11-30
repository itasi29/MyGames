#pragma once
#include <memory>
#include "Vec2.h"

class Input;
class StageBase;

/// <summary>
/// �X�e�[�W�̑J�ڂ��R���g���[��
/// </summary>
class StageManager
{
public:
	// FIXME:���O�͕ς���
	enum MoveDir
	{
		kDirLeft,
		kDirRight,
		kDirUp,
		kDirDown,
	};

	StageManager();
	~StageManager();

	void Update(Input& input);
	void Draw();

	/// <summary>
	///  �X�e�[�W�̐؂�ւ�
	/// </summary>
	/// <param name="nextStage">���̃X�e�[�W</param>
	void ChangeStage(std::shared_ptr<StageBase> nextStage);

	/// <summary>
	/// �X�e�[�W�����̂Ƃ��܂ł��܂��������悤
	/// �R�����g�ς����珑������
	/// </summary>
	/// <param name="pos">�������x�N�g��</param>
	/// <param name="handle">�摜�n���h��</param>
	void StartMove(MoveDir dir, int handle);

private:
	std::shared_ptr<StageBase> m_stage;

	// �摜�𓮂����Ă���t���[��
	int m_frame;
	// ��ʂ𓮂����Ă��邩
	bool m_isStageMove;
	// ��ʂ𓮂����x�N�g��
	Vec2 m_vec;
	// �ꏊ
	Vec2 m_pos;
	// ��ʂ̉摜�n���h��
	int m_stageHandle;

	void UpdateMove();
	void DrawMove();
};

