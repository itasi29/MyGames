#pragma once
#include "Scene.h"

class FileBase;
class SoundSystem;

/// <summary>
/// 音量調整シーン
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
	/// ゲージの描画
	/// </summary>
	/// <param name="drawX">左端描画位置</param>
	/// <param name="drawY">上端描画位置</param>
	/// <param name="rate">ボリューム割合</param>
	void DrawGauge(int drawX, int drawY, float rate);

private:
	updateFunc_t m_updateFunc;

	int m_currentLineIndex;
	bool m_isEdit;

	int m_frame;

	// 音関係
	std::shared_ptr<SoundSystem> m_soundSys;
	// 選択時Se
	std::shared_ptr<FileBase> m_selectSe;
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;
};

