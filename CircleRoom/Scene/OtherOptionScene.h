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
	std::shared_ptr<FileBase> m_rightNotationImg;
	// Se
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;

	int m_currentLineIndex;

	int m_frame;
};

