#pragma once
#include <memory>
#include "Scene.h"

class FileBase;

/// <summary>
/// �������̈��ʂ��ʂ��p�̃N���X
/// </summary>
class OneShotScene : public Scene
{
public:
	/// <summary>
	/// �C���X�g���N�^
	/// </summary>
	/// <param name="mgr">�Q�[���}�l�[�W���[</param>
	/// <param name="handle">�ʂ������摜�̃n���h��</param>
	OneShotScene(GameManager& mgr, int handle);
	~OneShotScene();

	void Update(Input& input);
	void Draw() const;

private:
	void AppearUpdate(Input& input);
	void NormalUpdate(Input& input);
	void DisAppearUpdate(Input& input);

	void MoveDraw() const;
	void NormalDraw() const;

private:
	using UpdateFunc_t = void(OneShotScene::*)(Input&);
	using DrawFunc_t = void(OneShotScene::*)() const;
	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	std::shared_ptr<FileBase> m_selectSe;

	int m_handle;
	int m_frame;

	// �摜�T�C�Y�Œ艻�p
	double m_sizeW;
	double m_sizeH;
};

