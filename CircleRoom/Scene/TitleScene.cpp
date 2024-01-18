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

// お試し用
#include "FileSystem/ImageFile.h"
#include "OneShotScene.h"

namespace
{
	// メニューラインの数
	constexpr int kMenuLineNum = 3;
	// ライン間隔
	constexpr float kMenuLineInterval = 64;
	// ラインの長さ
	constexpr int kMenuLength = 256;

	// 点滅間隔

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
	OutputDebugString(L"タイトルシーンがdeleteされました\n");
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
	if (m_frame <= 0) // 遷移条件
	{
		// 次の遷移先
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
		// 現在のラインの位置をメニューのラインの数で繰り返す
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
		// 0番目のときはスタート処理
		if (m_currentLinePos == 0)
		{
			m_updateFunc = &TitleScene::FadeOutUpdate;
			m_drawFunc = &TitleScene::FadeDraw;
			m_frame = 0;
			return;
		}
		// 1番目のときはオプション処理
		if (m_currentLinePos == 1)
		{
			// TODO:オプション画面を開く処理
			m_mgr.GetScene()->PushScene(std::make_shared<OptionScene>(m_mgr, input, false));
		}
		// 2番目のときは終了処理
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
	// 通常の方の描画
	NormalDraw();

	const auto& m_size = Application::GetInstance().GetWindowSize();
	// その後にフェード暗幕を描画
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, m_size.w, m_size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{
	// FIXME:マジックナンバーは後で直す
	// FIXME:書く順番考える

	const auto& m_size = Application::GetInstance().GetWindowSize();
	int drawX = m_size.w / 2;

	DrawBg(m_size);

	// ロゴの描画
	DrawGraph(0, 0, m_logoImg->GetHandle(), true);
	
	int y = static_cast<int>(184 + m_currentLinePos * kMenuLineInterval);
	// メニューラインの描画
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
