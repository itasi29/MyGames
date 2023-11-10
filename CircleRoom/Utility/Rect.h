#pragma once
#include "Vec2.h"

/// <summary>
/// 円を管理するためのクラス
/// </summary>
class Rect
{
public:
	Rect();
	virtual ~Rect();

	/// <summary>
	/// 当たり判定の描画
	/// </summary>
	/// <param name="color">色</param>
	/// <param name="isFill">true:全体描画 / false:外枠のみ描画</param>
	void Draw(unsigned int color, bool isFill) const;
	
	/// <summary>
	/// 中心地の更新
	/// </summary>
	/// <param name="pos">中心地</param>
	/// <param name="radius">半径</param>
	void SetCenter(Vec2 pos, float radius);

	// 必要な情報を取得する
	float GetRadius() const;
	Vec2 GetCenter() const;		// 矩形の中心座標

	/// <summary>
	/// 矩形同士の当たり判定
	/// </summary>
	/// <param name="target">判定側</param>
	/// <returns>true : 当たった, false : 当たってない</returns>
	bool IsCollsion(const Rect& target);

private:
	// 中心座標
	Vec2 m_pos;
	// 半径
	float m_radius;
};

