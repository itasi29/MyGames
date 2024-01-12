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
		"dash",		// É_ÉbÉVÉÖ
		"OK",		// ëIëorämíË
		"cancel",	// ÉLÉÉÉìÉZÉã
		"pause",	// É|Å[ÉYÉ{É^Éì
		"keyconf"	// ÉLÅ[ÉRÉìÉtÉBÉOÉ{É^Éì
	};

	m_bottanTable[PAD_INPUT_A] = L"Ç`Bottan";
	m_bottanTable[PAD_INPUT_B] = L"ÇaBottan";
	m_bottanTable[PAD_INPUT_C] = L"ÇbBottan";
	m_bottanTable[PAD_INPUT_X] = L"ÇwBottan";
	m_bottanTable[PAD_INPUT_Y] = L"ÇxBottan";
	m_bottanTable[PAD_INPUT_Z] = L"ÇyBottan";
	m_bottanTable[PAD_INPUT_L] = L"ÇkBottan";
	m_bottanTable[PAD_INPUT_R] = L"ÇqBottan";
	m_bottanTable[PAD_INPUT_START] = L"ÇrÇsÇ`ÇqÇsBottan";
	m_bottanTable[PAD_INPUT_M] = L"ÇlBottan";
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
