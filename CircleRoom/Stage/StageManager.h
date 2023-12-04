#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "Vec2.h"

class Input;
class StageBase;
struct Size;

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

	int GetSlideVolumeX(MoveDir dir) const;
	int GetSlideVolumeY(MoveDir dir) const;

	/// <summary>
	/// �N���A����ۑ�����
	/// </summary>
	/// <param name="path">�p�X</param>
	void Save(const std::string& path);

	/// <summary>
	/// �N���A����ǂݍ���
	/// </summary>
	/// <param name="path">�p�X</param>
	void Load(const std::wstring& path);


	void SaveClearInf(const std::string& stgName, const std::vector<bool>& inf);

	/// <summary>
	/// �N���A���������Ă���
	/// </summary>
	/// <param name="stgName">�X�e�[�W��</param>
	/// <param name="inf">�N���A���̔z��</param>
	/// <returns>true:�擾�ł��� / false:�擾�ł��Ȃ�����</returns>
	bool GetClearInf(const std::string& stgName, std::vector<bool>& inf);

private:
	// �X�e�[�W�̃N���A���Q
	std::unordered_map<std::string, std::vector<bool>> m_stageClearTable;

	// �X�e�[�W�̃|�C���^
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

	void ResetVecX();
	void ResetVecY();

private:
	const Size& m_size;
};

