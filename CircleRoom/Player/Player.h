#pragma once
#include "../Utility/Vec2.h"
#include "../Utility/Rect.h"

class Input;
class Application;
class Size;

class Player
{
public:
	Player(Application& app);
	~Player();

	/// <summary>
	/// プレイヤーの位置、方向の初期化
	/// </summary>
	void Init();
	void Update(Input& input);
	void Draw();

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	/// <returns>中心座標</returns>
	Vec2 GetPos() const { return m_pos; }

	/// <summary>
	/// 当たり判定の中心座標を取得
	/// </summary>
	/// <returns>当たり判定の中心座標</returns>
	Rect GetColPos() const { return m_colRect; }
	/// <summary>
	/// 当たり判定の半径を取得
	/// </summary>
	/// <returns>当たり判定の半径</returns>
	float GetColRadius() const { return m_colRaidus; }

private:
	// アプリケーションクラス
	Application& m_app;
	// Windowsのサイズ
	Size& m_size;

	// 中心座標
	Vec2 m_pos;
	// 前座標
	Vec2 m_frontPos;
	// 右後ろ座標
	Vec2 m_rightPos;
	// 左後ろ座標
	Vec2 m_leftPos;

	// 移動ベクトル
	Vec2 m_vec;

	// 当たり判定
	Rect m_colRect;
	// 当たり判定の半径
	float m_colRaidus;

	// 生存しているか
	bool m_isExsit;

	/// <summary>
	/// プレイヤーの移動
	/// </summary>
	/// <param name="input">入力情報</param>
	void Move(Input& input);
};

