#pragma once
class FileBase;
#include "Scene.h"
class OtherOptionScene : public Scene
{
public:
	OtherOptionScene(GameManager& mgr);
	~OtherOptionScene();

	void Update(Input& input);
	void Draw();

private:
	void FadeUpdate(Input& input);
	void NormalUpdate(Input& input);

	void FadeDraw();
	void NormalDraw();

private:
	using UpdateFunc_t = void(OtherOptionScene::*)(Input&);
	using DrawFunc_t = void(OtherOptionScene::*)();

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	// ‰æ‘œ
	std::shared_ptr<FileBase> m_frame;
	std::shared_ptr<FileBase> m_rightNotationImg;
	// Se
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;

	int m_currentLineIndex;

	int m_fadeFrame;
};

