#pragma once
#include "Scene.h"
#include <unordered_map>
#include <memory>
#include "Input.h"

class BottansFile;
class KeyFile;
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
	void EditEndUpdate(Input&);		// 編集終了状態

	void DrawCommandList();	// コマンドリストの描画(テキスト描画)

	void CommitCurrenKeySetting();

	std::wstring GetKeyName(int keycode);

private:
	UpdateFunc_t  m_updateFunc;

	// Optionのシーン
	std::shared_ptr<SceneManager> m_optionScn;

	// KEYの画像用
	std::shared_ptr<KeyFile> m_keyImg;

	// 特定の純情にコマンドを並び替えるための配列
	std::vector<std::string> m_menuTable;

	// Inputクラスの参照を持っておく
	Input& m_input;
	// Inputクラスのテーブル
	InputTable_t m_commandTable;

	// 現在行のインデックス
	int m_currentLineIndex;
	// 編集中
	bool m_isEdit;
	int m_frame;


	// MEMO:これは変更させる可能性高い
	std::unordered_map<int, std::wstring> m_keynameTable;
};

