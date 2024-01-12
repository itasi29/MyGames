#include <DxLib.h>
#include "Input.h"

#include "PadConfigScene.h"

namespace
{
	constexpr unsigned int kDefColor = 0xffffff;
	constexpr int kMenuMargin = 120;
}

PadConfigScene::PadConfigScene(GameManager& mgr, Input& input, std::shared_ptr<SceneManager> scn) :
	Scene(mgr),
	m_optionScn(scn),
	m_input(input),
	m_currentLineIndex(0)
{
	m_commandTable = input.GetCommandTable();

	m_menuTable = {
		"dash",		// �_�b�V��
		"OK",		// �I��or�m��
		"cancel",	// �L�����Z��
		"pause",	// �|�[�Y�{�^��
		"keyconf"	// �L�[�R���t�B�O�{�^��
	};

	m_bottanTable[PAD_INPUT_A] = L"�`Bottan";
	m_bottanTable[PAD_INPUT_B] = L"�aBottan";
	m_bottanTable[PAD_INPUT_C] = L"�bBottan";
	m_bottanTable[PAD_INPUT_X] = L"�wBottan";
	m_bottanTable[PAD_INPUT_Y] = L"�xBottan";
	m_bottanTable[PAD_INPUT_Z] = L"�yBottan";
	m_bottanTable[PAD_INPUT_L] = L"�kBottan";
	m_bottanTable[PAD_INPUT_R] = L"�qBottan";
	m_bottanTable[PAD_INPUT_START] = L"�r�s�`�q�sBottan";
	m_bottanTable[PAD_INPUT_M] = L"�lBottan";
}

PadConfigScene::~PadConfigScene()
{
}

void PadConfigScene::Update(Input& input)
{
}

void PadConfigScene::Draw()
{
	DrawString(100, kMenuMargin + 10, L"PadConfigScene", 0xffffff);

	DrawCommandList();
}

void PadConfigScene::DrawCommandList()
{
	int y = kMenuMargin + 64;
	for (const auto& itme : m_menuTable)
	{
		auto& cmd = m_commandTable[itme];
	}
}
