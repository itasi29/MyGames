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
	/// ����������
	/// </summary>
	/// <param name="isMove">�w�i�𓮂����� true : ������, false : �������Ȃ�</param>
	void Init(bool isMove);

private:
	void NormarUpdate();
	void MoveUpdate();

	/// <summary>
	/// ���������ɕ`��
	/// </summary>
	void NormalDraw() const;
	/// <summary>
	/// �������ĕ`��
	/// </summary>
	void MoveDraw() const;

	/// <summary>
	/// �֐��|�C���^��̕ύX
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

