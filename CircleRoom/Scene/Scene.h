#pragma once
class Input;
class SceneManager;
class StageManager;

/// <summary>
/// シーン基底クラス
/// いろんなシーンの基になるクラス
/// </summary>
class Scene
{
protected:
	SceneManager& m_scnMgr;
	StageManager& m_stgMgr;
public:
	/// <summary>
	/// 生成時にSceneManager, StageManagerの参照を受け取っておく
	/// </summary>
	/// <param name="manager">SceneManagerの参照</param>
	/// <param name="manager">StageManagerの参照</param>
	Scene(SceneManager& scnMgr, StageManager& stgMgr);
	virtual ~Scene() {};

	/// <summary>
	/// シーンの更新
	/// </summary>
	/// <param name="input">入力オブジェクト</param>
	virtual void Update(Input& input) = 0;
	/// <summary>
	/// シーンの描画
	/// </summary>
	virtual void Draw() = 0;
};

