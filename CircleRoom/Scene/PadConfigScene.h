#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "Scene.h"
#include "Input.h"

class KeyFile;
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

	/// <summary>
	/// 選択等の文字・画像描画
	/// </summary>
	/// /// <param name="cmd">コマンド名</param>
	/// <param name="str">ウェーブさせる文字列</param>
	/// <param name="num">文字列数</param>
	void DrawWave(const char* const cmd, const wchar_t* const str[], int num);

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
	int m_fadeFrame;
	// キャンセル確認フレーム
	int m_cancleFrame;

	// PADの番号と名前の連携
	std::unordered_map<int, std::wstring> m_bottanTable;

	// 画像
	std::shared_ptr<FileBase> m_frame;
	std::shared_ptr<FileBase> m_addFrame;
	std::shared_ptr<FileBase> m_startFrame;
	// Se
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;

	// 文字ウェーブ用の角度
	float m_waveAngle;
	// ウエーブのやつ描画するか
	bool m_isWaveDraw;

	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;
};

