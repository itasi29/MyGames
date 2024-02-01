#pragma once
#include "Scene.h"
#include <memory>
#include <array>

struct size;
class Input;
class FileBase;
class BottansFile;
class KeyFile;
class SceneManager;
enum class InputType;

/// <summary>
/// ポーズ時のシーンクラス
/// </summary>
class OptionScene : public Scene
{
public:
	OptionScene(GameManager& mgr, Input& input, bool isGame = true);

	void Update(Input& input);
	void Draw();

	void ChangeScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// m_isEditのフラグを反転させる
	/// </summary>
	void InverseIsEdit() { m_isEdit[0] = !m_isEdit[0]; }
private:
	// 更新メンバ関数ポインタ
	using UpdateFunc_t = void(OptionScene::*)(Input& input);
	UpdateFunc_t  m_updateFunc;

	// 更新関数
	void AppearUpdate(Input&);	// 登場状態
	void NormalUpdate(Input&);		// 通常状態
	void DisappearUpdate(Input&);	// 退場状態

	// 描画関数
	void NormalDraw();	// 非フェード描画
	
	/// <summary>
	/// 左右移動させる画像の描画
	/// </summary>
	void DrawWave(const size& size);
	/// <summary>
	/// 枠線の描画
	/// </summary>
	void DrawFrame(int divisionNum, int width, const size& size);
	/// <summary>
	/// 内容の文字列描画
	/// </summary>
	void DrawContent(std::vector<std::wstring> strs, int width);

	/// <summary>
	/// メニューを変更したときに何を描画するかを変更する
	/// </summary>
	void ChangeScene(Input& input);

private:
	// ゲームシーンとタイトルシーン変更させる
	bool m_isGame;

	// 
	std::shared_ptr<BottansFile> m_bt;
	std::shared_ptr<KeyFile> m_key;

	// シーンの扱い方が特殊なのでSceneManager別枠として持つ
	// おかしくなる可能性があるため要確認
	std::shared_ptr<SceneManager> m_optionScn;

	int m_frame = 0;
	std::array<bool, 2> m_isEdit;

	int m_currentMenuLine;

	bool m_isFadeOut;

	InputType m_type;

	// Se
	std::shared_ptr<FileBase> m_cursorUpSe;
	std::shared_ptr<FileBase> m_cursorDownSe;
};

