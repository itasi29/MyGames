#pragma once
#include "Scene.h"

class FileBase;
class SoundSystem;

/// <summary>
/// ���ʒ����V�[��
/// </summary>
class SoundOptionScene :
    public Scene
{
public:
    SoundOptionScene(GameManager& mgr);
    ~SoundOptionScene();

    void Update(Input& input);
    void Draw();

private:
	using updateFunc_t = void(SoundOptionScene::*)(Input&);

	void NormalUpdate(Input& input);
	void EditUpdate(Input& input);

	void DrawName(int drawY, int index, std::wstring str);

	/// <summary>
	/// �Q�[�W�̕`��
	/// </summary>
	/// <param name="drawX">���[�`��ʒu</param>
	/// <param name="drawY">��[�`��ʒu</param>
	/// <param name="rate">�{�����[������</param>
	void DrawGauge(int drawX, int drawY, float rate);

private:
	updateFunc_t m_updateFunc;

	int m_currentLineIndex;
	bool m_isEdit;

	int m_frame;

	// ���֌W
	std::shared_ptr<SoundSystem> m_soundSys;
	// �I����Se
	std::shared_ptr<FileBase> m_selectSe;
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;
};

