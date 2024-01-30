#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/FontSystem.h"
#include "Input.h"

#include "OptionScene.h"

#include "SoundOptionScene.h"

// ここはフレームの描画の仕方が違うから後で確認

namespace
{
	// フレームの色
	constexpr unsigned int kFrameColor = 0xd2001a;
	// フレーム点滅時の色の差
	constexpr unsigned int kFrameColorDeff = 0x225036;

	// 通常文字列の色
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// 選択時文字列の色
	constexpr unsigned int kYellowColor = 0xffde00;
	// 点滅間隔
	constexpr int kFlashInterval = 40;

	// フレームの左余白
	constexpr int kFrameMargin = 16;

	constexpr int kAppeaInterval = 5;
	constexpr int kMenuMargin = 120;

	constexpr int kMenuLineInterval = 128;

	// 音声リピート間隔
	constexpr int kRepeatInterval = 6;

	// ゲージの長さ
	constexpr int kGaugeLength = 100;

	enum
	{
		kBgm,
		kSe,
		kMax
	};
}

SoundOptionScene::SoundOptionScene(GameManager& mgr) :
	Scene(mgr),
	m_currentLineIndex(0),
	m_isEdit(false),
	m_fadeFrame(0),
	m_repeatUpFrame(0),
	m_repeatDownFrame(0)
{
	m_updateFunc = &SoundOptionScene::NormalUpdate;

	m_soundSys = mgr.GetSound();
	auto& file = m_mgr.GetFile();
	m_frame = file->LoadGraphic(L"UI/normalFrame.png", true);
	m_addFrame = file->LoadGraphic(L"UI/addFrame.png");

	m_selectSe = file->LoadSound(L"Se/select.mp3", true);
	m_cursorUpSe = file->LoadSound(L"Se/cursorUp.mp3", true);
	m_cursorDownSe = file->LoadSound(L"Se/cursorDown.mp3", true);
}

SoundOptionScene::~SoundOptionScene()
{
}

void SoundOptionScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void SoundOptionScene::Draw()
{	
	int y = kMenuMargin + 36 + m_currentLineIndex * kMenuLineInterval;

	// 選択している場所を描画
	DrawGraph(kMenuMargin + 800, y, m_frame->GetHandle(), true);
	DrawBox(128 - kFrameMargin, y,
		kMenuMargin + 800, y + 44,
		kFrameColor, true);

	// 選択中の場合は色を追加して点滅させる
	if (m_isEdit)
	{
		int frame = (m_fadeFrame % (kFlashInterval * 2)) - kFlashInterval;
		float rate = fabsf(static_cast<float>(frame)) / kFlashInterval;
		int add = static_cast<int>(255 * rate);
		SetDrawBlendMode(DX_BLENDMODE_ADD, add);
		DrawGraph(kMenuMargin + 800, y, m_addFrame->GetHandle(), true);
		DrawBox(128, y,
			kMenuMargin + 800, y + 40,
			kFrameColorDeff, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	y = kMenuMargin + 42;
	auto rate = m_mgr.GetSound()->GetBgmVolRate();
	DrawName(y, kBgm, L"BGM");
	DrawFormatStringToHandle(200, y, kWhiteColor, fontHandle, L"%3d％", static_cast<int>(rate * 100));
	DrawGauge(500, y, rate);

	y += kMenuLineInterval;
	rate = m_mgr.GetSound()->GetSeVolRate();
	DrawName(y, kSe, L"SE");
	DrawFormatStringToHandle(200, y, kWhiteColor, fontHandle, L"%3d％", static_cast<int>(rate * 100));
	DrawGauge(500, y, rate);
}

void SoundOptionScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		m_soundSys->PlaySe(m_selectSe->GetHandle());
		m_isEdit = true;
		m_fadeFrame = kFlashInterval;
		std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
		optionScene->InverseIsEdit();

		m_updateFunc = &SoundOptionScene::EditUpdate;
	}

	m_fadeFrame++;

	if (input.IsTriggered("up"))
	{
		m_currentLineIndex = (m_currentLineIndex - 1 + kMax) % kMax;
		m_fadeFrame = 0;
		m_sound->PlaySe(m_cursorUpSe->GetHandle());
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % kMax;
		m_fadeFrame = 0;
		m_sound->PlaySe(m_cursorDownSe->GetHandle());
	}
}

void SoundOptionScene::EditUpdate(Input& input)
{
	if (input.IsTriggered("OK"))
	{
		m_isEdit = false;
		std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
		optionScene->InverseIsEdit();

		m_updateFunc = &SoundOptionScene::NormalUpdate;
	}


	if (input.IsReepat("right", m_repeatUpFrame, kRepeatInterval))
	{
		switch (m_currentLineIndex)
		{
		default:
			assert(false);
		case kBgm:
			m_mgr.GetSound()->ChangeBgmVol(10);
			break;

		case kSe:
			m_mgr.GetSound()->ChangeSeVol(10);
			break;
		}
	}
	if (input.IsReepat("left", m_repeatDownFrame, kRepeatInterval))
	{
		switch (m_currentLineIndex)
		{
		default:
			assert(false);
		case kBgm:
			m_mgr.GetSound()->ChangeBgmVol(-10);
			break;

		case kSe:
			m_mgr.GetSound()->ChangeSeVol(-10);
			break;
		}
	}

	m_fadeFrame++;
}

void SoundOptionScene::DrawName(int drawY, int index, std::wstring str)
{
	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	if (m_currentLineIndex == index)
	{
		if (!m_isEdit)
		{
			int frame = (m_fadeFrame % (kFlashInterval * 2)) - kFlashInterval;
			float rate = fabsf(static_cast<float>(frame)) / kFlashInterval;
			int alpha = static_cast <int>(255 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		}
		DrawStringToHandle(132, drawY, str.c_str(), kYellowColor, fontHandle);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		DrawStringToHandle(132, drawY, str.c_str(), kWhiteColor, fontHandle);
	}
}

void SoundOptionScene::DrawGauge(int drawX, int drawY, float rate)
{
	// 下地描画
	DrawBox(drawX, drawY, drawX + kGaugeLength, drawY + 32, 0xa9a9a9, true);
	
	// ゲージ割合描画
	DrawBox(drawX, drawY, drawX + static_cast<int>(kGaugeLength * rate), drawY + 32, 0xff9130, true);
}
