#pragma once
#include "Scene.h"
#include <unordered_map>
#include <memory>
#include "Input.h"

class BottansFile;
class SceneManager;

/// <summary>
/// キーコンフィグのシーン
/// </summary>
class KeyConfigScene : public Scene
{
public:
	KeyConfigScene(GameManager& mgr, Input& input, std::shared_ptr<SceneManager> scn);
	~KeyConfigScene();

	void Update(Input& input);
	void Draw();

private:
	// 更新メンバ関数ポインタ
	using UpdateFunc_t = void(KeyConfigScene::*)(Input& input);
	
	// 更新関数
	void NormalUpdate(Input&);		// 通常状態
	void EditUpdate(Input&);		// 編集状態

	// 描画関数
	void ExpandDraw();	// 拡張縮張描画
	void NormalDraw();	// 非フェード描画

	void DrawCommandList();	// コマンドリストの描画(テキスト描画)

	void CommitCurrenKeySetting();

	std::wstring GetKeyName(int keycode);
	std::wstring GetPadName(int padstate);

private:
	UpdateFunc_t  m_updateFunc;

	std::shared_ptr<SceneManager> m_optionScn;

	int m_frame = 0;
	std::vector<std::string> m_menuItems;	// 特定の純情にコマンドを並び替えるための配列

	int m_currentLineIndex = 0;		// 現在行のインデックス
	bool m_isEditingNow = false;	// 今編集中なのか(トグル)

	Input& m_input;	// Inputクラスの参照を持っておく
	InputTable_t m_keyCommandTable;

	bool m_isEditRequestButton;

	std::shared_ptr<BottansFile> m_btImg;

	// MEMO:これは変更させる可能性高い
	std::unordered_map<int, std::wstring> m_keynameTable;
	std::unordered_map<int, std::wstring> m_bottans;
};

