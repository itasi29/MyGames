#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "Common/Input.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "GamePlayingScene.h"

namespace
{
	// メニューラインの数
	constexpr int kMenuLineNum = 3;
	// ライン間隔
	constexpr float kMenuLineInterval = 64;
	// ラインの長さ
	constexpr int kMenuLength = 256;
}

TitleScene::TitleScene(SceneManager& scnMgr, StageManager& stgMgr) :
	Scene(scnMgr, stgMgr)
{
	m_frame = 60;
	m_updateFunc = &TitleScene::FadeInUpdate;
	m_drawFunc = &TitleScene::FadeDraw;
}

TitleScene::~TitleScene()
{
	OutputDebugString(L"タイトルシーンがdeleteされました\n");
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
	if (m_frame <= 0) // 遷移条件
	{
		// 次の遷移先
		m_updateFunc = &TitleScene::NormalUpdate;
		m_drawFunc = &TitleScene::NormalDraw;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("up"))
	{
		// 現在のラインの位置をメニューのラインの数で繰り返す
		m_currentLinePos = (kMenuLineNum + m_currentLinePos - 1) % kMenuLineNum;
	}
	if (input.IsTriggered("down"))
	{
		m_currentLinePos = (m_currentLinePos + 1) % kMenuLineNum;
	}

	if (input.IsTriggered("OK"))
	{
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
		}
		// 2番目のときは終了処理
		if (m_currentLinePos == 2)
		{
			// TODO:終了を伝える処理
		}
	}
}

void TitleScene::FadeOutUpdate(Input&)
{
	m_frame++;
	if (60 <= m_frame)
	{
		m_scnMgr.ChangeScene(std::make_shared<GamePlayingScene>(m_scnMgr, m_stgMgr));
	}
}

void TitleScene::FadeDraw()
{
	// 通常の方の描画
	NormalDraw();

	const auto& size = Application::GetInstance().GetWindowSize();
	// その後にフェード暗幕を描画
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / 60.0f));
	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, size.w, size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{
	// FIXME:マジックナンバーは後で直す
	// FIXME:書く順番考える

	const auto& size = Application::GetInstance().GetWindowSize();
	int defX = size.w / 2;
		
	// タイトル名の描画
	std::wstring title = L"CircleRoom";
	float strLen = static_cast<float>(title.size());
	DrawExtendString(static_cast<int>(defX - (strLen / 2)) * 16, 100,
		2, 2, 
		title.data(), 0xffffff);

	int y = static_cast<int>(200 + m_currentLinePos * kMenuLineInterval);
	// メニューラインの描画
	DrawLine(defX, y,
		defX + kMenuLength, y, 
		0xff0808);

	// スタート
	DrawString(defX, 200-16, L"START", 0xffffff);
	// オプション
	DrawString(defX, 264-16, L"OPTION", 0xffffff);
	// 終了
	DrawString(defX, 328-16, L"END", 0xffffff);
}
