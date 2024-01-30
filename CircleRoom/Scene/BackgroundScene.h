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
	/// <param name="speed">速度(何も入れなければデフォルトスピードで動く)</param>
	void Init(float speed = -1);

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

	std::shared_ptr<FileBase> m_bg;

	float m_scroll;
	float m_scrollSpeed;
	bool m_isMove;

	bool m_isDraw;
};

