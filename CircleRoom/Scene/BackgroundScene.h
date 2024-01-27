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
	/// 初期化処理
	/// </summary>
	/// <param name="isMove">背景を動かすか true : 動かす, false : 動かさない</param>
	void Init(bool isMove);

private:
	void NormarUpdate();
	void MoveUpdate();

	/// <summary>
	/// 動かさずに描画
	/// </summary>
	void NormalDraw() const;
	/// <summary>
	/// 動かして描画
	/// </summary>
	void MoveDraw() const;

	/// <summary>
	/// 関数ポインタ先の変更
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

