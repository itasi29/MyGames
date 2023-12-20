#pragma once
#include "Scene.h"

class Input;

/// <summary>
/// ポーズ時のシーンクラス
/// </summary>
class PauseScene : public Scene
{
public:
	PauseScene(GameManager& mgr);

	void Update(Input& input);
	void Draw();

private:
	// 更新メンバ関数ポインタ
	using UpdateFunc_t = void(PauseScene::*)(Input& input);
	UpdateFunc_t  m_updateFunc;
	// 描画メンバ関数ポインタ
	using DrawFunc_t = void (PauseScene::*)();
	DrawFunc_t m_drawFunc;

	// 更新関数
	void AppearUpdate(Input&);	// 登場状態
	void NormalUpdate(Input&);		// 通常状態
	void DisappearUpdate(Input&);	// 退場状態

	// 描画関数
	void ExpandDraw();	// 拡張縮張描画
	void FadeDraw();	// フェード描画
	void NormalDraw();	// 非フェード描画

private:
	int m_frame = 0;
	bool m_title;

	int m_currentMenuLine;
};

