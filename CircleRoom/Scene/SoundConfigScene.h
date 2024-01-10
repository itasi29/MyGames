#pragma once
#include "Scene.h"

/// <summary>
/// 音量調整シーン
/// </summary>
class SoundConfigScene :
    public Scene
{
public:
    SoundConfigScene(GameManager& mgr);
    ~SoundConfigScene();

    void Update(Input& input);
    void Draw();

private:

	/// <summary>
	/// ゲージの描画
	/// </summary>
	/// <param name="drawX">左端描画位置</param>
	/// <param name="drawY">上端描画位置</param>
	/// <param name="rate">ボリューム割合</param>
	void DrawGauge(int drawX, int drawY, float rate);

private:
	int m_frame = 0;
};

