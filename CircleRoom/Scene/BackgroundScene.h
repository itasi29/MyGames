#pragma once
#include <vector>
#include <memory>

class FileBase;
class FileManager;

class BackgroundScene
{
public:
	BackgroundScene(std::shared_ptr<FileManager> mgr, bool isMove = true, bool isDraw = true);
	~BackgroundScene();

	void Update();
	void Draw() const;

	/// <summary>
	/// ‰Šú‰»ˆ—
	/// </summary>
	/// <param name="isMove">”wŒi‚ğ“®‚©‚·‚© true : “®‚©‚·, false : “®‚©‚³‚È‚¢</param>
	void Init(bool isMove);

private:
	void NormarUpdate();
	void MoveUpdate();

	/// <summary>
	/// “®‚©‚³‚¸‚É•`‰æ
	/// </summary>
	void NormalDraw() const;
	/// <summary>
	/// “®‚©‚µ‚Ä•`‰æ
	/// </summary>
	void MoveDraw() const;

	/// <summary>
	/// ŠÖ”ƒ|ƒCƒ“ƒ^æ‚Ì•ÏX
	/// </summary>
	void ChangeFunc();

private:
	using UpdateFunc_t = void(BackgroundScene::*)();
	using DrawFunc_t = void(BackgroundScene::*)() const;

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	std::vector<std::shared_ptr<FileBase>> m_bg;

	int m_scroll;
	bool m_isMove;

	bool m_isDraw;
};

