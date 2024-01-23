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
	void StartSelectUpdate(Input&);	// 続きからor初めから
	void FadeOutUpdate(Input&);	// フェードアウト状態

	// 描画関数
	void FadeDraw();	// フェード中描画
	void NormalDraw();	// 非フェード描画
	void StartSelectDraw();	// スタート選択状態描画

private:
	/// <summary>
	/// ロゴの描画
	/// </summary>
	void DrawLogo();

private:
	// 更新メンバ関数ポインタ
	void(TitleScene::* m_updateFunc)(Input& input);
	// 描画メンバ関数ポインタ
	using DrawFunc_t = void (TitleScene::*)();
	DrawFunc_t m_drawFunc;

	// 画像群
	std::shared_ptr<FileBase> m_logoImg;
	std::shared_ptr<FileBase> m_bgImg;

	// 全体フレーム
	int m_frame;
	// メニュー選択カーソル位置
	int m_currentLinePos;

	// ロゴアングル
	float m_logoAngle;

	// 背景フレーム
	int m_bgFrame;

	// 音関係
	std::shared_ptr<SoundSystem> m_soundSys;
	// Bgm
	std::shared_ptr<FileBase> m_bgm;
	// 選択時Se
	std::shared_ptr<FileBase> m_selectSe;
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;
};

