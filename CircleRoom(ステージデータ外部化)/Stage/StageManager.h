#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "Vec2.h"

class Input;
class SceneManager;
class StageBase;
class StageMap;
class GameData;
struct size;

enum class StageDir
{
	kStageLeft,
	kStageRight,
	kStageUp,
	kStageDown,
	kStageMax,
};

/// <summary>
/// �X�e�[�W�̑J�ڂ��R���g���[��
/// </summary>
class StageManager
{
public:

	StageManager(std::shared_ptr<SceneManager>& mgr);
	~StageManager();

	void Init(Input& input);
	void InitPos();

	void Update(Input& input);
	void Draw() const;

	std::string GetStageName() const;

	bool IsClear() const { return m_isClear; }
	void OnClear() { m_isClear = true; }
	void ResetClear() { m_isClear = false; }
	void OffDrawWave() const;

	std::shared_ptr<StageMap> GetMap() const { return m_map; }
	std::shared_ptr<GameData> GetData() const { return m_data; }

	/// <summary>
	///  �X�e�[�W�̐؂�ւ�
	/// </summary>
	/// <param name="nextStage">���̃X�e�[�W��</param>
	/// <param name="isGameScene">�Q�[���V�[������̕ύX��</param>
	void ChangeStage(const std::string& nextStage, bool isGameScene = false);

	/// <summary>
	/// �X�e�[�W�̑����ύX
	/// </summary>
	void ImmediatelyChange();

private:
	// �ʏ�̍X�V
	void NormalUpdate(Input& input);
	// �X�e�[�W�ړ��̍X�V
	void StageMoveUpdate(Input& input);
	// �ړ����v���C�̍X�V
	void MoveGamePlaingUpdate(Input& input);

	// �ʏ�̕`��
	void NormalDraw() const;
	// �X�e�[�W�ړ����̕`��
	void StageMoveDraw() const;
	// �ړ����v���C�̕`��
	void MoveGamePlaingDraw() const;

	void CheckEnd();

private:
	using UpdateFunc_t = void(StageManager::*)(Input&);
	using DrawFunc_t = void(StageManager::*)() const;

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	const size& m_size;

	std::shared_ptr<SceneManager>& m_mgr;

	// ��ʕ`��p�n���h��
	int m_drawScreen;
	// �ړ����̉�ʃn���h��
	int m_screen;
	// �L�[�v�p��ʃn���h��
	int m_keepScreen;

	// �Q�[���N���A
	bool m_isClear;

	// �X�e�[�W�̃|�C���^
	std::shared_ptr<StageBase> m_stage;
	// �}�b�v���̃|�C���^
	std::shared_ptr<StageMap> m_map;
	// �Q�[���f�[�^�̃|�C���^
	std::shared_ptr<GameData> m_data;

	// ��ʂ������Ă��邩
	bool m_isMove;

	// ��ʂ𓮂����x�N�g��
	Vec2 m_vec;
	// ���݂̃X�e�[�W�̏ꏊ
	Vec2 m_pos;
	// ���̃X�e�[�W�̏ꏊ
	Vec2 m_targetPos;
};

