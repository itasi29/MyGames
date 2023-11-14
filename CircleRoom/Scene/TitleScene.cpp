#include <DxLib.h>
#include <cassert>
#include "../Common/Input.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "GamePlayingScene.h"

TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager)
{
	m_frame = 60;
	m_updateFunc = &TitleScene::FadeInUpdate;
	m_drawFunc = &TitleScene::FadeDraw;
}

TitleScene::~TitleScene()
{
	OutputDebugString(L"�^�C�g���V�[����delete����܂���\n");
}

void TitleScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void TitleScene::Draw()
{
	(this->*m_drawFunc)();
}

void TitleScene::FadeInUpdate(Input&)
{
	m_frame--;
	if (m_frame <= 0) // �J�ڏ���
	{
		// ���̑J�ڐ�
		m_updateFunc = &TitleScene::NormalUpdate;
		m_drawFunc = &TitleScene::NormalDraw;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		m_updateFunc = &TitleScene::FadeOutUpdate;
		m_drawFunc = &TitleScene::FadeDraw;
		m_frame = 0;
	}
}

void TitleScene::FadeOutUpdate(Input&)
{
	m_frame++;
	if (60 <= m_frame)
	{
		m_manager.ChangeScene(std::make_shared<GamePlayingScene>(m_manager));
	}
}

void TitleScene::FadeDraw()
{
	// �ʏ�̕`��
	DrawString(10, 100, L"TitleScene", 0xffffff);

	// �t�F�[�h�Ö�
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, 640, 480, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{
	DrawString(10, 100, L"TitleScene", 0xffffff);
}
