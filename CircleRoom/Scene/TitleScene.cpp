#include <DxLib.h>
#include <cassert>
#include <string>
#include "Application.h"
#include "Common/Input.h"

#include "GameManager.h"
#include "SceneManager.h"
#include "FileSystem/BottansFile.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/FileManager.h"

#include "GamePlayingScene.h"
#include "OptionScene.h"

#include "TitleScene.h"

// �������p
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

	// �_�ŊԊu

	const std::wstring kMenuStr[kMenuLineNum] = {
		L"START",
		L"OPTION",
		L"END"
	};
}

TitleScene::TitleScene(GameManager& mgr) :
	Scene(mgr),
	m_bgFrame(0)
{
	m_frame = 60;
	m_updateFunc = &TitleScene::FadeInUpdate;
	m_drawFunc = &TitleScene::FadeDraw;

	auto& file = mgr.GetFile();
	m_logoImg = file->LoadGraphic(L"logo.png");
	m_bgImg = file->LoadGraphic(L"BG/bg.png");

	m_soundSys = mgr.GetSound();
	m_selectSe = file->LoadSound(L"Se/select.mp3");
}

TitleScene::~TitleScene()
{
	OutputDebugString(L"�^�C�g���V�[����delete����܂���\n");
}

void TitleScene::Update(Input& input)
{
	m_bgFrame++;
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

		m_frame = 0;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	m_frame++;

	if (input.IsTriggered("up"))
	{
		// ���݂̃��C���̈ʒu�����j���[�̃��C���̐��ŌJ��Ԃ�
		m_currentLinePos = (kMenuLineNum + m_currentLinePos - 1) % kMenuLineNum;
		m_frame = 0;
	}
	if (input.IsTriggered("down"))
	{
		m_currentLinePos = (m_currentLinePos + 1) % kMenuLineNum;
		m_frame = 0;
	}

	if (input.IsTriggered("OK"))
	{
		m_soundSys->PlaySe(m_selectSe->GetHandle());
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

	DrawBg(m_size);

	// ���S�̕`��
	DrawGraph(0, 0, m_logoImg->GetHandle(), true);
	
	int y = static_cast<int>(184 + m_currentLinePos * kMenuLineInterval);
	// ���j���[���C���̕`��
	DrawBox(drawX, y, drawX + kMenuLength, y + 32, 0xff0808, true);

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	y = 200 - 16;

	for (int i = 0; i < kMenuLineNum; i++)
	{
		if (m_currentLinePos == i)
		{
			int frame = (m_frame % 80) - 40;
			float rate = fabs(frame) / 40.0f;
			int alpha = 255 * rate;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawStringToHandle(drawX, y, kMenuStr[i].c_str(), 0xffffff, fontHandle);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			DrawStringToHandle(drawX, y, kMenuStr[i].c_str(), 0xffffff, fontHandle);
		}

		y += 64;
	}
}

void TitleScene::DrawBg(const size& size)
{
	int posX = m_bgFrame % size.w;
	DrawGraph(posX, 0, m_bgImg->GetHandle(), false);
	posX -= size.w;
	DrawGraph(posX, 0, m_bgImg->GetHandle(), false);
}
