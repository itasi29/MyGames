#pragma once
#include "../Utility/Vec2.h"
#include "../Utility/Rect.h"

class Input;
class Application;
struct Size;

class Player
{
public:
	Player(const Size& windowSize, float fieldSize);
	~Player();

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
	// スクリーンサイズ
	const Size& m_windowSize;
	// フィールドのサイズ
	float m_fieldSize;

	// 中心座標
	Vec2 m_pos;
	// 正面ベクトル
	Vec2 m_frontVec;
	// 右ベクトル
	Vec2 m_rightVec;
	// 左ベクトル
	Vec2 m_leftVec;
	// 現在の正面方向
	Vec2 m_nowFront;

	// 当たり判定
	Rect m_colPos;

	// 移動ベクトル
	Vec2 m_vec;
	// ダッシュするフレーム
	int m_dashFrame;
	// ダッシュ再利用可能までのフレーム
	int m_dashWaitFrame;
	// ダッシュ判定
	bool m_isDash;

	// 当たり判定
	Rect m_colRect;
	// 当たり判定の半径
	float m_colRaidus;

	// 生存しているか
	bool m_isExsit;

private:
	/// <summary>
	/// プレイヤーの移動
	/// </summary>
	/// <param name="input">入力情報</param>
	void Move(Input& input);
	/// <summary>
	/// ダッシュ処理
	/// </summary>
	/// <param name="input">入力情報</param>
	void Dash(Input& input);
	/// <summary>
	/// 場外判定処理
	/// </summary>
	void InRange();
};

