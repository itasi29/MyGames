#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "Common/Input.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "FileSystem/BottansFile.h"

#include "GamePlayingScene.h"
#include "OptionScene.h"

#include "TitleScene.h"

// �������p
#include "FileSystem/FileManager.h"
#include "FileSystem/ImageFile.h"
#include "OneShotScene.h"

namespace
{
	// ���j���[���C���̐�
	constexpr int kMenuLineNum = 3;
	// ���C���Ԋu
	constexpr float kMenuLineInterval = 64;
	// ���C���̒���
	constexpr int kMenuLength = 256;
}

TitleScene::TitleScene(GameManager& mgr) :
	Scene(mgr)
{
	m_frame = 60;
	m_updateFunc = &TitleScene::FadeInUpdate;
	m_drawFunc = &TitleScene::FadeDraw;

	m_logoImg = m_mgr.GetFile()->LoadGraphic(L"logo.png");
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
	if (input.IsTriggered("up"))
	{
		// ���݂̃��C���̈ʒu�����j���[�̃��C���̐��ŌJ��Ԃ�
		m_currentLinePos = (kMenuLineNum + m_currentLinePos - 1) % kMenuLineNum;
	}
	if (input.IsTriggered("down"))
	{
		m_currentLinePos = (m_currentLinePos + 1) % kMenuLineNum;
	}

	if (input.IsTriggered("OK"))
	{
		// 0�Ԗڂ̂Ƃ��̓X�^�[�g����
		if (m_currentLinePos == 0)
		{
			m_updateFunc = &TitleScene::FadeOutUpdate;
			m_drawFunc = &TitleScene::FadeDraw;
			m_frame = 0;
			return;
		}
		// 1�Ԗڂ̂Ƃ��̓I�v�V��������
		if (m_currentLinePos == 1)
		{
			// TODO:�I�v�V������ʂ��J������
			m_mgr.GetScene()->PushScene(std::make_shared<OptionScene>(m_mgr, input, false));
		}
		// 2�Ԗڂ̂Ƃ��͏I������
		if (m_currentLinePos == 2)
		{
			Application& app = Application::GetInstance();
			app.End();
		}
	}
}

void TitleScene::FadeOutUpdate(Input&)
{
	m_frame++;
	if (60 <= m_frame)
	{
		m_mgr.GetScene()->ChangeScene(std::make_shared<GamePlayingScene>(m_mgr));
	}
}

void TitleScene::FadeDraw()
{
	// �ʏ�̕��̕`��
	NormalDraw();

	const auto& m_size = Application::GetInstance().GetWindowSize();
	// ���̌�Ƀt�F�[�h�Ö���`��
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, m_size.w, m_size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{
	// FIXME:�}�W�b�N�i���o�[�͌�Œ���
	// FIXME:�������ԍl����

	const auto& m_size = Application::GetInstance().GetWindowSize();
	int drawX = m_size.w / 2;

	// ���S�̕`��
	DrawGraph(0, 0, m_logoImg->GetHandle(), true);
		
	// �^�C�g�����̕`��
	std::wstring title = L"CircleRoom";
	float strLen = static_cast<float>(title.size());
	DrawExtendString(static_cast<int>(drawX - (strLen / 2)) * 16, 100,
		2, 2, 
		title.data(), 0xffffff);

	int y = static_cast<int>(200 + m_currentLinePos * kMenuLineInterval);
	// ���j���[���C���̕`��
	DrawLine(drawX, y,
		drawX + kMenuLength, y, 
		0xff0808);

	// �X�^�[�g
	DrawString(drawX, 200-16, L"START", 0xffffff);
	// �I�v�V����
	DrawString(drawX, 264-16, L"OPTION", 0xffffff);
	// �I��
	DrawString(drawX, 328-16, L"END", 0xffffff);
}
