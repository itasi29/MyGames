#include <DxLib.h>
#include <cassert>
#include <string>
#include "Common/Input.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FileBase.h"
#include "FileSystem/SoundSystem.h"
#include "GameClearScene.h"
#include "TitleScene.h"

#ifdef _DEBUG
#include "Application.h"
#endif

namespace
{
	// 文字列の色
	constexpr unsigned int kStrColor = 0xf0ece5;

	// フェード時間
	constexpr int kFadeFrame = 60;

	// リザルト数
	constexpr int kResultNum = 3;

	// 文字のX座標描画位置
	constexpr int kDrawResultX = 350;
	// リザルト結果のX座標描画位置
	constexpr int kDrawResultDataX = 750;

	// 文字同士の間隔
	constexpr int kTextInterval = 180;

	const std::wstring kResultStr[kResultNum] = {
		L"クリア時間",
		L"死亡数",
		L"ダッシュ使用回数"
	};
}

GameClearScene::GameClearScene(GameManager& mgr) :
	Scene(mgr),
	m_frame(kFadeFrame),
	m_textFrame(0),
	m_index(0)
{
	m_updateFunc = &GameClearScene::FadeInUpdate;
	m_drawFunc = &GameClearScene::FadeDraw;

	auto& file = mgr.GetFile();
	m_bg = file->LoadGraphic(L"BG/bg.png");
	m_bgm = file->LoadSound(L"Bgm/end.mp3");
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Update(Input& input)
{
	m_bgFrame--;
	(this->*m_updateFunc)(input);
}

void GameClearScene::Draw()
{
	(this->*m_drawFunc)();
}

void GameClearScene::FadeInUpdate(Input&)
{
	m_sound->PlayFadeBgm(m_bgm->GetHandle(), 1.0f - (m_frame / static_cast<float>(kFadeFrame)));
	m_frame--;
	if (m_frame <= 0)
	{
		m_updateFunc = &GameClearScene::NormalUpdate;
		m_drawFunc = &GameClearScene::NormalDraw;

		m_frame = 0;
	}
}

void GameClearScene::NormalUpdate(Input& input)
{
	m_sound->PlayBgm(m_bgm->GetHandle());
	m_textFrame++;

	if (m_index < kResultNum)
	{
		if (m_textFrame > kTextInterval || input.IsTriggered("OK"))
		{
			m_textFrame = 0;
			m_index++;
		}

		return;
	}

	if (input.IsAnyTriggerd())
	{
		m_updateFunc = &GameClearScene::FadeOutUpdate;
		m_drawFunc = &GameClearScene::FadeDraw;

		m_frame = 0;
	}
}

void GameClearScene::FadeOutUpdate(Input&)
{
	m_sound->PlayFadeBgm(m_bgm->GetHandle(), 1.0f - (m_frame / static_cast<float>(kFadeFrame)));
	m_frame++;
	if (kFadeFrame <= m_frame)
	{
		m_sound->Stop(m_bgm->GetHandle());
		m_mgr.GetScene()->ChangeScene(std::make_shared<TitleScene>(m_mgr));
	}
}

void GameClearScene::FadeDraw()
{
	NormalDraw();

	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameClearScene::NormalDraw()
{
	DrawStringToHandle(550, 50, L"リザルト", kStrColor, m_mgr.GetFont()->GetHandle(48));
	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	int drawY = 150;
	for (int i = 0; i < m_index + 1; i++)
	{
		if (!(i < kResultNum))
		{
			int val = (m_textFrame + kTextInterval) % (kTextInterval * 2) - kTextInterval;
			float rate = (fabs(static_cast<float>(val)) / kTextInterval);
			int alpha = static_cast <int>(255 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawStringToHandle(500, drawY, L"～　PressAnyKey　～", kStrColor, fontHandle);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			break;
		}

		if (i == m_index)
		{
			float rate = (static_cast<float>(m_textFrame) / kTextInterval);
			int alpha = static_cast <int>(255 * rate);
			int y = drawY - static_cast<int>(100 * (1 - rate));

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawFormatStringToHandle(kDrawResultX, y, kStrColor, fontHandle, L"%s", kResultStr[i].c_str());
			DrawInf(i, y, fontHandle);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			DrawFormatStringToHandle(kDrawResultX, drawY, kStrColor, fontHandle, L"%s", kResultStr[i].c_str());
			DrawInf(i, drawY, fontHandle);
		}

		drawY += 100;
	}
}

void GameClearScene::DrawInf(int index, int drawY, int handle)
{
	const auto& data = m_mgr.GetData();

	if (index == 0)
	{
		int sec = (data.playTime / 60) % 60;
		int min = (data.playTime / 3600) % 60;
		int hour = (data.playTime / 21600);
		DrawFormatStringToHandle(kDrawResultDataX, drawY, kStrColor, handle, L"%02d時間%02d分%02d秒", hour, min, sec);
	}
	else if (index == 1)
	{
		DrawFormatStringToHandle(kDrawResultDataX, drawY, kStrColor, handle, L"%-3d回", data.deathCount);
	}
	else
	{
		DrawFormatStringToHandle(kDrawResultDataX, drawY, kStrColor, handle, L"%-3d回", data.dashCount);
	}
}