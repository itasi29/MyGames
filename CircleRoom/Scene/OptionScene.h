#pragma once
#include "Scene.h"
#include <memory>

class Input;
class SceneManager;

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
	void InverseIsEdit() { m_isEdit = !m_isEdit; }

	bool IsEdit() const { return m_isEdit; }

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

	void DrawContent(std::vector<std::wstring> strs, int width);

	/// <summary>
	/// メニューを変更したときに何を描画するかを変更する
	/// </summary>
	void ChangeScene(Input& input);

private:
	// ゲームシーンとタイトルシーン変更させる
	bool m_isGame;

	// シーンの扱い方が特殊なのでSceneManager別枠として持つ
	// おかしくなる可能性があるため要確認
	std::shared_ptr<SceneManager> m_optionScn;

	int m_frame = 0;
	bool m_isEdit;

	int m_currentMenuLine;

	bool m_isFadeOut;
};

