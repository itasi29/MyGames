#pragma once
#include "Scene.h"
#include <unordered_map>
#include "Common/Input.h"
/// <summary>
/// キーコンフィグのシーン
/// </summary>
class KeyConfigScene : public Scene
{
public:
	KeyConfigScene(SceneManager& mgr, Input& input);
	~KeyConfigScene();

	void Update(Input& input);
	void Draw();

private:
	int m_frame = 0;
	std::vector<std::string> m_menuItems;	// 特定の純情にコマンドを並び替えるための配列

	int m_currentLineIndex = 0;		// 現在行のインデックス
	bool m_isEditingNow = false;	// 今編集中なのか(トグル)

	Input& m_input;	// Inputクラスの参照を持っておく
	InputTable_t m_keyCommandTable;

	// 更新メンバ関数ポインタ
	using UpdateFunc_t = void(KeyConfigScene::*)(Input& input);
	UpdateFunc_t  m_updateFunc;
	// 描画メンバ関数ポインタ
	using DrawFunc_t = void (KeyConfigScene::*)();
	DrawFunc_t m_drawFunc;

	// 更新関数
	void AppearUpdate(Input&);	// 登場状態
	void NormalUpdate(Input&);		// 通常状態
	void EditUpdate(Input&);		// 編集状態
	void DisappearUpdate(Input&);	// 退場状態

	// 描画関数
	void ExpandDraw();	// 拡張縮張描画
	void NormalDraw();	// 非フェード描画

	void DrawCommandList();	// コマンドリストの描画(テキスト描画)

	void CommitCurrenKeySetting();
};

