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
	/// <param name="speed">���x(��������Ȃ���΃f�t�H���g�X�s�[�h�œ���)</param>
	void Init(float speed = -1);

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

	std::shared_ptr<FileBase> m_bg;

	float m_scroll;
	float m_scrollSpeed;
	bool m_isMove;

	bool m_isDraw;
};

