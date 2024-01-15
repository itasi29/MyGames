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
	void NormalUpdate(Input& input);
	void EditUpdate(Input& input);
	void EditEndUpdate(Input& input);

	void DrawCommandList();

	std::wstring GetPadName(int padstate);

private:
	using updateFunc_t = void (PadConfigScene::*)(Input&);
	updateFunc_t m_updateFunc;

	// optionのシーン
	std::shared_ptr<SceneManager> m_optionScn;

	// PADの画像用
	std::shared_ptr<BottansFile> m_btImg;

	// メニューの並ぶ順用
	std::vector<std::string> m_menuTable;

	// Inputクラスを元から持っておく
	Input& m_input;
	// Inputクラスのテーブル
	InputTable_t m_commandTable;

	// 選択位置
	int m_currentLineIndex;
	// 編集中
	bool m_isEdit;
	int m_frame;

	// PADの番号と名前の連携
	std::unordered_map<int, std::wstring> m_bottanTable;
};

