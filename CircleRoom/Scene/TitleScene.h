#pragma once
#include <memory>
#include "Scene.h"

class FileBase;
class SoundSystem;
struct size;

/// <summary>
/// タイトルシーンクラス
/// </summary>
class TitleScene : public Scene
{
public:
	TitleScene(GameManager& mgr);
	~TitleScene();
	virtual void Update(Input& input);
	virtual void Draw();

private:
	// 更新関数
	void FadeInUpdate(Input&);	// フェードイン状態
	void NormalUpdate(Input&);	// 通常状態
	void FadeOutUpdate(Input&);	// フェードアウト状態

	// 描画関数
	void FadeDraw();	// フェード中描画
	void NormalDraw();	// 非フェード描画

private:
	void DrawBg(const size& size);

private:
	int m_frame = 0;
	// メニュー選択カーソル位置
	int m_currentLinePos = 0;

	// 更新メンバ関数ポインタ
	void(TitleScene::* m_updateFunc)(Input& input);
	// 描画メンバ関数ポインタ
	using DrawFunc_t = void (TitleScene::*)();
	DrawFunc_t m_drawFunc;

	// タイトルロゴ
	std::shared_ptr<FileBase> m_logoImg;
	// 背景画像
	std::shared_ptr<FileBase> m_bgImg;

	// 背景フレーム
	int m_bgFrame;

	// 音関係
	std::shared_ptr<SoundSystem> m_soundSys;
	// 選択時Se
	std::shared_ptr<FileBase> m_selectSe;
};

