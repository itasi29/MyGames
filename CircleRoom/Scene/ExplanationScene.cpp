#include <DxLib.h>
#include <cassert>
#include <vector>
#include <string>
#include <sstream>
#include "Application.h"
#include "Input.h"
#include "GameManager.h"
#include "Scene/SceneManager.h"
#include "Scene/OneShotScene.h"
#include "FileSystem/FileManager.h"
#include "FileSystem/FontSystem.h"
#include "FileSystem/SoundSystem.h"
#include "FileSystem/BottansFile.h"
#include "FileSystem/KeyFile.h"
#include "FileSystem/FileBase.h"

#include "OptionScene.h"
#include "OtherOptionScene.h"
#include "ExplanationScene.h"

namespace
{
	// �t���[���̐F
	constexpr unsigned int kFrameColor = 0xd2001a;
	// �ʏ핶����̐F
	constexpr unsigned int kWhiteColor = 0xf0ece5;
	// �I����������̐F
	constexpr unsigned int kYellowColor = 0xffde00;;
	// �_�ŊԊu
	constexpr int kFlashInterval = 40;
	// X���W�`��
	constexpr int kDrawX = 132;
	// �t���[����
	constexpr int kFrameWidht = 788;
	// �t���[���̍��]��
	constexpr int kFrameMargin = 20;

	constexpr int kMenuMargin = 120;
	// �����Ԃ̕�
	constexpr int kMenuLineInterval = 96;

	enum
	{
		kOperation,
		kProceed,
		kDash,
		kBoss
	};

	const std::vector<std::wstring> kGameMenu = { L"����ɂ���",
		L"�i�ߕ��ɂ���",
		L"�_�b�V���ɂ���",
		L"�{�X�̓|�����ɂ���"
	};

	// �X�^�[�g�����̃E�F�[�u�X�s�[�h
	constexpr float kWaveSpeed = DX_PI_F / 180 * 5;
	// �E�F�[�u�̊Ԋu
	constexpr float kWaveInterval = DX_PI_F / 15.0f;

	// �E�F�[�u������
	int kSelectWaveNum = 4;
	int kSelectWavePosX = 1064;
	int kSelectWavePosY = 592;
	const wchar_t* const kSelectWave[] = { L"��", L"��", L"��", L"��" };

	int kBackWaveNum = 3;
	int kBackWavePosX = 1128;
	int kBackWavePosY = 544;
	const wchar_t* const kBackWave[] = { L"��", L"��", L"��" };
}

ExplanationScene::ExplanationScene(GameManager& mgr, Input& input, std::shared_ptr<SceneManager> scn) :
	Scene(mgr),
	m_input(input),
	m_optionScn(scn),
	m_currentLineIndex(0),
	m_fadeFrame(0),
	m_index(0),
	m_waveAngle(DX_PI_F),
	m_isWaveDraw(true)
{
	std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
	optionScene->InverseIsEdit();

	auto& file = m_mgr.GetFile();
	m_frame = file->LoadGraphic(L"UI/normalFrame.png", true);
	m_startFrame = file->LoadGraphic(L"UI/startFrame.png");
	m_operator = file->LoadGraphic(L"UI/operationExplanation.png");
	for (int i = 0; i < 3; i++)
	{
		std::wostringstream woss;
		woss << L"UI/explanation" << i << L".png";
		m_proceed[i] = file->LoadGraphic(woss.str());
	}
	m_dash = file->LoadGraphic(L"UI/dashExplanation.png");
	m_boss = file->LoadGraphic(L"UI/bossExplanation.png");

	m_selectSe = file->LoadSound(L"Se/select.mp3", true);

	m_bt = std::make_shared<BottansFile>(file);
	m_key = std::make_shared<KeyFile>(file);

	const auto& size = Application::GetInstance().GetWindowSize();
	m_updateFunc = &ExplanationScene::SelectUpdate;
}

ExplanationScene::~ExplanationScene()
{
	std::shared_ptr<OptionScene > optionScene = std::dynamic_pointer_cast<OptionScene>(m_mgr.GetScene()->GetTopScene());
	optionScene->InverseIsEdit();
}

void ExplanationScene::Update(Input& input)
{
	(this->*m_updateFunc)(input);
}

void ExplanationScene::Draw() const
{
	// �I�����Ă���ꏊ��`��
	int y = kMenuMargin + 36 + m_currentLineIndex * kMenuLineInterval;

	DrawGraph(kDrawX + kFrameWidht, y, m_frame->GetHandle(), true);
	DrawBox(kDrawX - kFrameMargin, y,
		kDrawX + kFrameWidht, y + 44,
		kFrameColor, true);

	int fontHandle = m_mgr.GetFont()->GetHandle(32);

	y = kMenuMargin + 42;

	// ���j���[�̕�����Q
	for (int i = 0; i < kGameMenu.size(); i++)
	{
		if (m_currentLineIndex == i)
		{
			int frame = (m_fadeFrame % (kFlashInterval * 2)) - kFlashInterval;
			float rate = fabsf(static_cast<float>(frame)) / kFlashInterval;
			int alpha = static_cast <int>(255 * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawStringToHandle(kDrawX, y, kGameMenu[i].c_str(), kYellowColor, fontHandle);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			DrawStringToHandle(kDrawX, y, kGameMenu[i].c_str(), kWhiteColor, fontHandle);
		}

		y += kMenuLineInterval;
	}

	DrawWave(kBackWavePosX, kBackWavePosY, "cancel", kBackWave, kBackWaveNum);
	DrawWave(kSelectWavePosX, kSelectWavePosY, "OK", kSelectWave, kSelectWaveNum);
}

void ExplanationScene::SelectUpdate(Input& input)
{
	m_isWaveDraw = true;
	m_waveAngle -= kWaveSpeed;
	m_fadeFrame++;

	if (input.IsTriggered("cancel"))
	{
		m_optionScn->ChangeScene(std::make_shared<OtherOptionScene>(m_mgr, input, m_optionScn));
	}

	if (input.IsTriggered("up"))
	{
		m_currentLineIndex = (m_currentLineIndex - 1 + static_cast<int>(kGameMenu.size())) % static_cast<int>(kGameMenu.size());
		m_fadeFrame = 0;
	}
	if (input.IsTriggered("down"))
	{
		m_currentLineIndex = (m_currentLineIndex + 1) % kGameMenu.size();
		m_fadeFrame = 0;
	}

	if (input.IsTriggered("OK"))
	{
		auto& sound = GameManager::GetInstance().GetSound();
		sound->PlaySe(m_selectSe->GetHandle());
		m_isWaveDraw = false;
		m_index = 0;
		switch (m_currentLineIndex)
		{
		default:
			assert(false);
		case kOperation:
			m_updateFunc = &ExplanationScene::OperationUpdate;
			break;

		case kProceed:
			m_updateFunc = &ExplanationScene::ProceedUpdate;
			break;

		case kDash:
			m_updateFunc = &ExplanationScene::DashUpdate;
			break;

		case kBoss:
			m_updateFunc = &ExplanationScene::BossUpdate;
			break;
		}
	}
}

void ExplanationScene::OperationUpdate(Input&)
{
	if (m_index >= 1)
	{
		m_updateFunc = &ExplanationScene::SelectUpdate;
		return;
	}

	m_mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(m_mgr, m_operator->GetHandle()));
	m_index++;
}

void ExplanationScene::ProceedUpdate(Input&)
{
	if (m_index >= 3)
	{
		m_updateFunc = &ExplanationScene::SelectUpdate;
		return;
	}

	m_mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(m_mgr, m_proceed[m_index]->GetHandle()));
	m_index++;
}

void ExplanationScene::DashUpdate(Input&)
{
	if (m_index >= 1)
	{
		m_updateFunc = &ExplanationScene::SelectUpdate;
		return;
	}

	m_mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(m_mgr, m_dash->GetHandle()));
	m_index++;
}

void ExplanationScene::BossUpdate(Input&)
{
	if (m_index >= 1)
	{
		m_updateFunc = &ExplanationScene::SelectUpdate;
		return;
	}

	m_mgr.GetScene()->PushScene(std::make_shared<OneShotScene>(m_mgr, m_boss->GetHandle()));
	m_index++;
}

void ExplanationScene::DrawWave(int x, int y, const char* const cmd, const wchar_t* const str[], int num) const
{
	if (!m_isWaveDraw) return;

	DrawGraph(x - 84, y - 5, m_startFrame->GetHandle(), true);

	switch (m_input.GetType())
	{
	case InputType::keybd:
		m_key->DrawKey(m_input.GetHardDataName(cmd, InputType::keybd), x - 48, y, 2.0);
		break;
	default:
		assert(false);
	case InputType::pad:
		m_bt->DrawBottan(m_input.GetHardDataName(cmd, InputType::pad), x - 48, y, 2.0);
		break;
	}

	int handle = m_mgr.GetFont()->GetHandle(32);

	int strX = x;

	for (int i = 0; i < num; i++)
	{
		int add = static_cast<int>(sinf(m_waveAngle + kWaveInterval * i) * -10);

		if (add > 0)
		{
			add = 0;
		}

		int strY = y + add;


		DrawStringToHandle(strX, strY, str[i], kWhiteColor, handle);
		strX += 24;
	}
}
