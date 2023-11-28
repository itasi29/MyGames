#pragma once
#include <vector>
#include <memory>

class StageManager;
class Player;
class EnemyBase;
class Input;
struct Size;

class StageBase
{
public:
	StageBase(std::shared_ptr<StageManager> mgr, const Size& windowSize, float fieldSize);
	virtual ~StageBase();

	void Update(Input& input);
	void Draw();

	virtual void Init() = 0;
	virtual void CreateEnemy() = 0;

	/// <summary>
	/// �X�e�[�W��ύX����
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <returns>true : ����ւ��\/ fasle : ����ւ��s�\</returns>
	virtual void ChangeStage(Input& input) = 0;

protected:
	std::shared_ptr<StageManager> m_mgr;

	// Window�T�C�Y
	const Size& m_windowSize;
	// �t�B�[���h�T�C�Y
	float m_fieldSize;

	// �v���C���[
	std::shared_ptr<Player> m_player;
	// �G
	std::vector<std::shared_ptr<EnemyBase>> m_enemy;

	// �o�ߎ���
	int m_frame;

protected:
	using UpdateFunc_t = void (StageBase::*)(Input&);
	using DrawFunc_t = void (StageBase::*)();

	/// <summary>
	/// �I�𒆂̍X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	void UpdateSelect(Input& input);
	/// <summary>
	/// �v���C���̍X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	void UpdatePlaying(Input& input);

	/// <summary>
	/// �I�𒆂̕`�揈��
	/// </summary>
	void DrawSelect();
	/// <summary>
	/// �v���C���̕`�揈��
	/// </summary>
	void DrawPlaying();

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;
};

