#pragma once
class Input;
class GameManager;

/// <summary>
/// シーン基底クラス
/// いろんなシーンの基になるクラス
/// </summary>
class Scene
{
protected:
	GameManager& m_mgr;
public:
	/// <summary>
	/// 生成時にGameManagerの参照を受け取っておく
	/// </summary>
	/// <param name="manager">SceneManagerの参照</param>
	Scene(GameManager& mgr);
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

