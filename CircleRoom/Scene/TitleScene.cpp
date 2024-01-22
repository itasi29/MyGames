#include <DxLib.h>
#include <cassert>
#include <string>
#include "Application.h"
#include "Common/Input.h"

#include "GameManager.h"
#include "SceneManager.h"
#include "Stage/StageManager.h"
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
	// �t�F�[�h�t���[��
	constexpr int kFadeFrame = 60;

	// ���j���[���C���̐�
	constexpr int kMenuLineNum = 3;
	// ���C���Ԋu
	constexpr float kMenuLineInterval = 128;
	// ���C���̒���
	constexpr int kMenuLength = 256;

	// �_�ŊԊu
	constexpr int kFlashInterval = 40;

	// �������X���W
	constexpr int kStrDrawX = 640 + 256;

	// ���S�`��ʒu
	constexpr int kLogoDrawX = 374;
	constexpr int kLogoDrawY = 360;

	const std::wstring kMenuStr[kMenuLineNum] = {
		L"�X�^�[�g",
		L"�I�v�V����",
		L"�G���h"
	};

	// �X�^�[�g�Z���N�g
	constexpr int kStartSelectNum = 2;

	const std::wstring kStartSelect[kStartSelectNum] =
	{
		L"��������",
		L"���߂���"
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
	m_bgm = file->LoadSound(L"Bgm/title.mp3");
	m_selectSe = file->LoadSound(L"Se/select.mp3", true);
	m_cursorUpSe = file->LoadSound(L"Se/cursorUp.mp3", true);
	m_cursorDownSe = file->LoadSound(L"Se/cursorDown.mp3", true);
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
	m_sound->PlayFadeBgm(m_bgm->GetHandle(), 1.0f - m_frame / static_cast<float>(kFadeFrame));
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
	m_sound->PlayBgm(m_bgm->GetHandle());
	m_frame++;

	if (input.IsTriggered("up"))
	{
		// ���݂̃��C���̈ʒu�����j���[�̃��C���̐��ŌJ��Ԃ�
		m_currentLinePos = (kMenuLineNum + m_currentLinePos - 1) % kMenuLineNum;
		m_frame = 0;
		m_sound->PlaySe(m_cursorUpSe->GetHandle());
	}
	if (input.IsTriggered("down"))
	{
		m_currentLinePos = (m_currentLinePos + 1) % kMenuLineNum;
		m_frame = 0;
		m_sound->PlaySe(m_cursorDownSe->GetHandle());
	}

	if (input.IsTriggered("OK"))
	{
		m_soundSys->PlaySe(m_selectSe->GetHandle());
		// 0�Ԗڂ̂Ƃ��̓X�^�[�g����
		if (m_currentLinePos == 0)
		{
			// �`���[�g���A�����N���A���Ă����瑱������or���߂���
			if (m_mgr.GetStage()->IsClearStage("StageTutorial"))
			{
				m_updateFunc = &TitleScene::StartSelectUpdate;
				m_drawFunc = &TitleScene::StartSelectDraw;
				m_frame = 0;
			}
			// ���ĂȂ���΋����X�^�[�g
			else
			{
				m_updateFunc = &TitleScene::FadeOutUpdate;
				m_drawFunc = &TitleScene::FadeDraw;
				m_frame = 0;

				// ��ʊO�ɔ�΂�
				m_currentLinePos = -10;
			}
			return;
		}
		// 1�Ԗڂ̂Ƃ��̓I�v�V��������
		if (m_currentLinePos == 1)
		{
			// TODO:�I�v�V������ʂ��J������
			m_mgr.GetScene()->PushScene(std::make_shared<OptionScene>(m_mgr, input, false));
			return;
		}
		// 2�Ԗڂ̂Ƃ��͏I������
		if (m_currentLinePos == 2)
		{
			Application& app = Application::GetInstance();
			app.End();
			return;
		}
	}
}

void TitleScene::StartSelectUpdate(Input& input)
{
	m_frame++;

	if (input.IsTriggered("cancel"))
	{
		m_updateFunc = &TitleScene::NormalUpdate;
		m_drawFunc = &TitleScene::NormalDraw;

		m_frame = 0;
	}

	if (input.IsTriggered("up"))
	{
		// ���݂̃��C���̈ʒu�����j���[�̃��C���̐��ŌJ��Ԃ�
		m_currentLinePos = (kStartSelectNum + m_currentLinePos - 1) % kStartSelectNum;
		m_frame = 0;
		m_sound->PlaySe(m_cursorUpSe->GetHandle());
	}
	if (input.IsTriggered("down"))
	{
		m_currentLinePos = (m_currentLinePos + 1) % kStartSelectNum;
		m_frame = 0;
		m_sound->PlaySe(m_cursorDownSe->GetHandle());
	}

	if (input.IsTriggered("OK"))
	{
		// ��������
		if (m_currentLinePos == 0)
		{
			// MEMO:���ɂ��邱�ƂȂ��Ǝv��
		}
		// ���߂���
		if (m_currentLinePos == 1)
		{
			// �f�[�^�̍폜
			m_mgr.GetStage()->DeleteData();
			m_mgr.DeleteData();
		}

		// ��ʊO�ɔ�΂�
		m_currentLinePos = -10;

		m_updateFunc = &TitleScene::FadeOutUpdate;
		m_drawFunc = &TitleScene::FadeDraw;
		m_frame = 0;

		return;
	}
}

void TitleScene::FadeOutUpdate(Input&)
{
	m_sound->PlayFadeBgm(m_bgm->GetHandle(), 1.0f - m_frame / static_cast<float>(kFadeFrame));
	m_frame++;
	if (m_frame > kFadeFrame)
	{
		m_sound->Stop();
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
	DrawBg(m_size);

	// ���S�̕`��
	DrawRotaGraph(kLogoDrawX, kLogoDrawY, 1.0, 0.0, m_logoImg->GetHandle(), true);
	
	int y = static_cast<int>(184 + m_currentLinePos * kMenuLineInterval);
	// ���j���[���C���̕`��
	DrawBox(kStrDrawX, y, kStrDrawX + kMenuLength, y + 32, 0xff0808, true);

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	y = 200 - 16;

	for (int i = 0; i < kMenuLineNum; i++)
	{
		if (m_currentLinePos == i)
		{
			int frame = (m_frame % kFlashInterval * 2) - kFlashInterval;
			float rate = fabsf(static_cast<float>(frame)) / static_cast<float>(kFlashInterval);
			int alpha = static_cast <int>(255 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawStringToHandle(kStrDrawX, y, kMenuStr[i].c_str(), 0xffffff, fontHandle);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			DrawStringToHandle(kStrDrawX, y, kMenuStr[i].c_str(), 0xffffff, fontHandle);
		}

		y += kMenuLineInterval;
	}
}

void TitleScene::StartSelectDraw()
{
	const auto& m_size = Application::GetInstance().GetWindowSize();
	DrawBg(m_size);

	// ���S�̕`��
	DrawRotaGraph(kLogoDrawX, kLogoDrawY, 1.0, 0.0, m_logoImg->GetHandle(), true);

	int y = static_cast<int>(212 + m_currentLinePos * 40);
	// �I���ꏊ�̕`��
	DrawBox(kStrDrawX + 64, y, kStrDrawX + 64 + kMenuLength, y + 40, 0xff0808, true);

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	y = 216;
	for (int i = 0; i < kStartSelectNum; i++)
	{
		if (m_currentLinePos == i)
		{
			int frame = (m_frame % kFlashInterval * 2) - kFlashInterval;
			float rate = fabsf(static_cast<float>(frame)) / static_cast<float>(kFlashInterval);
			int alpha = static_cast <int>(255 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawStringToHandle(kStrDrawX + 64, y, kStartSelect[i].c_str(), 0xffffff, fontHandle);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			DrawStringToHandle(kStrDrawX + 64, y, kStartSelect[i].c_str(), 0xffffff, fontHandle);
		}

		y += 40;
	}
	//DrawStringToHandle(kStrDrawX + 64, y, L"��������", 0xffffff, fontHandle);
	//y += 40;
	//DrawStringToHandle(kStrDrawX + 64, y, L"���߂���", 0xffffff, fontHandle);

	y = 184;
	for (int i = 0; i < kMenuLineNum; i++)
	{
		DrawStringToHandle(kStrDrawX, y, kMenuStr[i].c_str(), 0xffffff, fontHandle);

		y += kMenuLineInterval;
	}
}

void TitleScene::DrawBg(const size& size)
{
	int posX = m_bgFrame % size.w;
	DrawGraph(posX, 0, m_bgImg->GetHandle(), false);
	posX -= size.w;
	DrawGraph(posX, 0, m_bgImg->GetHandle(), false);
}
