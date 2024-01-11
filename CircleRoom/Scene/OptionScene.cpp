#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "Common/Input.h"

#include "OptionScene.h"

#include "GameManager.h"
#include "StageSelectScene.h"
#include "KeyConfigScene.h"
#include "SoundOptionScene.h"
#include "OtherOptionScene.h"
#include "TitleScene.h"

namespace
{
	// 
	constexpr int kAppeaInterval = 5;
	// �]��
	constexpr int kMenuMargin = 50;
	// �I��g�̏c��
	constexpr int kMenuMarginHeight = 32;
	// �I��
	constexpr int kMenuMarginWidth = 270;

	// �����Ԋu
	constexpr int kMenuStringMargin = 32;

	const std::vector<std::wstring> kMenuString = { L"�X�e�[�W�I��",
		L"�L�[�ݒ�",
		L"���ʐݒ�",
		L"���̑�" };

	enum
	{
		kStageSelect,
		kKey,
		kValume,
		kOther
	};
}

OptionScene::OptionScene(GameManager& mgr, Input& input) :
	Scene(mgr),
	m_isEdit(false),
	m_currentMenuLine(0),
	m_isFadeOut(false)
{
	m_updateFunc = &OptionScene::AppearUpdate;

	m_scnMgr = std::make_shared<SceneManager>();
	m_scnMgr->PushScene(std::make_shared<StageSelectScene>(m_mgr));
}

void OptionScene::Update(Input& input)
{
	m_scnMgr->Update(input);

	// �ҏW���͏����̕ύX�����Ȃ�
	if (m_isEdit) return;

	(this->*m_updateFunc)(input);
}

void OptionScene::Draw()
{
	// �t�F�[�h�A�E�g���͕`�悵�Ȃ�
	if (m_isFadeOut) return;

	// MEMO:�����ŕύX������\��������̂Ō��̂Ɏ����`�ŏ����Ă���
	// MEMO:�ύX�Ȃ���Ί֐����ނ̂�߂�
	NormalDraw();
	m_scnMgr->Draw();
}

void OptionScene::AppearUpdate(Input&)
{
	m_frame++;
	if (kAppeaInterval <= m_frame)
	{
		m_updateFunc = &OptionScene::NormalUpdate;
	}
}

void OptionScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("pause"))
	{
		m_updateFunc = &OptionScene::DisappearUpdate;
		m_isFadeOut = true;

		return;
	}

	if (input.IsTriggered("left"))
	{
		m_currentMenuLine = (m_currentMenuLine - 1 + static_cast<int>(kMenuString.size())) % static_cast<int>(kMenuString.size());
		ChangeScene(input);
	}
	if (input.IsTriggered("right"))
	{
		m_currentMenuLine = (m_currentMenuLine + 1) % kMenuString.size();
		ChangeScene(input);
	}
}

void OptionScene::DisappearUpdate(Input&)
{
	m_frame--;
	if (m_frame > 0) return;

	m_mgr.GetScene().PopScene();
}

void OptionScene::NormalDraw()
{
	Application& app = Application::GetInstance();
	const auto& m_size = app.GetWindowSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 126);
	// ������ƈÂ���`��`��
	DrawBox(kMenuMargin, kMenuMargin, m_size.w - kMenuMargin, m_size.h - kMenuMargin,
		0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// �g��
	DrawBox(kMenuMargin, kMenuMargin, m_size.w - kMenuMargin, m_size.h - kMenuMargin,
		0xffffff, false);

	// �I�����Ă���ꏊ��`��
	DrawBox(kMenuMargin * 2 + kMenuMarginWidth * m_currentMenuLine, static_cast<int>(kMenuMargin),
		kMenuMargin * 2 + kMenuMarginWidth * (m_currentMenuLine + 1), static_cast<int>(kMenuMargin + kMenuMarginHeight),
		0xff0000, true);
 

	// ���j���[�̕�����Q
	for (int i = 0; i < kMenuString.size(); i++)
	{
		if (m_currentMenuLine == i)
		{
			DrawString(kMenuMargin * 2 + kMenuMarginWidth * i, kMenuMargin, kMenuString[i].c_str(), 0x000000);
		}
		else
		{
			DrawString(kMenuMargin * 2 + kMenuMarginWidth * i, kMenuMargin, kMenuString[i].c_str(), 0xffffff);
		}
	}
}

void OptionScene::ChangeScene(Input& input)
{
	switch (m_currentMenuLine)
	{
		// �X�e�[�W�I��
	case kStageSelect:
		m_scnMgr->ChangeScene(std::make_shared<StageSelectScene>(m_mgr));
		break;

		// �L�[�ݒ�
	case kKey:
		m_scnMgr->ChangeScene(std::make_shared<KeyConfigScene>(m_mgr, input));
		break;

		// ���ʐݒ�
	case kValume:
		m_scnMgr->ChangeScene(std::make_shared<SoundOptionScene>(m_mgr));
		break;

		// ���̑��ݒ�
	default:
		assert(false);
	case kOther:
		m_scnMgr->ChangeScene(std::make_shared<OtherOptionScene>(m_mgr));
		break;
	}
}
