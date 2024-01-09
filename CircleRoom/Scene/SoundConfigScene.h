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
	// 更新メンバ関数ポインタ
	using UpdateFunc_t = void(SoundConfigScene::*)(Input& input);
	// 描画メンバ関数ポインタ
	using DrawFunc_t = void (SoundConfigScene::*)();

	// 更新関数
	void AppearUpdate(Input&);	// 登場状態
	void NormalUpdate(Input&);		// 通常状態
	void EditUpdate(Input&);		// 編集状態
	void DisappearUpdate(Input&);	// 退場状態

	// 描画関数
	void ExpandDraw();	// 拡張縮張描画
	void NormalDraw();	// 非フェード描画

	/// <summary>
	/// ゲージの描画
	/// </summary>
	/// <param name="drawX">左端描画位置</param>
	/// <param name="drawY">上端描画位置</param>
	/// <param name="rate">ボリューム割合</param>
	void DrawGauge(int drawX, int drawY, float rate);

private:
	UpdateFunc_t  m_updateFunc;
	DrawFunc_t m_drawFunc;

	int m_frame = 0;
};

