#pragma once
#include "Scene.h"

class FileBase;
class SoundSystem;

/// <summary>
/// ¹Ê²®V[
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
	/// Q[WÌ`æ
	/// </summary>
	/// <param name="drawX">¶[`æÊu</param>
	/// <param name="drawY">ã[`æÊu</param>
	/// <param name="rate">{[</param>
	void DrawGauge(int drawX, int drawY, float rate);

private:
	updateFunc_t m_updateFunc;

	int m_currentLineIndex;
	bool m_isEdit;

	int m_fadeFrame;

	// æÖW
	std::shared_ptr<FileBase> m_frame;
	std::shared_ptr<FileBase> m_addFrame;

	// ¹ÖW
	std::shared_ptr<SoundSystem> m_soundSys;
	// IðSe
	std::shared_ptr<FileBase> m_selectSe;
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;
};

