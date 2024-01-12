#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "Scene.h"
#include "Input.h"

class BottansFile;
class SceneManager;

/// <summary>
/// PADコンフィグのシーン
/// </summary>
class PadConfigScene : public Scene
{
public:
	PadConfigScene(GameManager& mgr, Input& input, std::shared_ptr<SceneManager> scn);
	~PadConfigScene();

	void Update(Input& input);
	void Draw();

private:
	void DrawCommandList();

private:
	// optionのシーン
	std::shared_ptr<SceneManager> m_optionScn;

	// メニューの並ぶ順用
	std::vector<std::string> m_menuTable;

	// Inputクラスを元から持っておく
	Input& m_input;
	// Inputクラスのテーブル
	InputTable_t m_commandTable;

	// 選択位置
	int m_currentLineIndex;

	// PADの番号と名前の連携
	std::unordered_map<int, std::wstring> m_bottanTable;
};

