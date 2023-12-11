#pragma once

#include <string>
#include "Vec2.h"
#include "Utility/Collision.h"

struct Size;

/// <summary>
/// Enemyクラスの基底
/// </summary>
class EnemyBase
{
public:
	EnemyBase(const Size& windowSize, float fieldSize);
	virtual ~EnemyBase();

	virtual void Init(const Vec2&) {}
	void Update();
	void Draw();

	/// <summary>
	/// 当たり判定の中心座標を取得
	/// </summary>
	/// <returns>当たり判定の中心座標</returns>
	Collision GetRect() const { return m_col; }

	/// <summary>
	/// 敵の名前を返す
	/// </summary>
	/// <returns>名前</returns>
	std::string GetName() const { return m_name; }

	/// <summary>
	/// 生存判定
	/// </summary>
	/// <returns>true: 生きてる / false:死んでる</returns>
	bool IsExsit() const { return m_isExsit; }

protected:
	/// <summary>
	/// 壁に当たったら反射させる
	/// </summary>
	/// <param name="scale">半径の大きさ</param>
	/// <param name="isShift">壁に当たった時に反射させるか</param>
	virtual bool Reflection(float scale = 0.0f, bool isShift = true);
	/// <summary>
	/// 反射させる計算
	/// </summary>
	void ReflectionCal(const Vec2& norVec);
	void ShiftReflection(const Vec2& shift);

	/// <summary>
	/// normal関数に変更する
	/// </summary>
	void ChangeNormalFunc();

protected:
	// メンバ関数ポインタ
	using updateFunc_t = void(EnemyBase::*)();
	using drawFunc_t = void(EnemyBase::*)();

	updateFunc_t m_updateFunc;
	drawFunc_t m_drawFunc;

	// 更新関数
	virtual void StartUpdate() = 0;
	virtual void NormalUpdate() = 0;

	// 描画関数
	virtual void StartDraw(); 
	virtual void NormalDraw();

protected:
	// 実体化するまでの時間
	static const int kApeearFrame = 60;

	// スクリーンサイズ
	const Size& m_windowSize;
	// フィールドのサイズ
	const float m_fieldSize;

	// 敵の名前
	std::string m_name;
	// 敵のカラー(グラフに変えたら消えるやつ)
	unsigned int m_color;

	// 中心座標
	Vec2 m_pos;
	// 移動ベクトル
	Vec2 m_vec;
	// 当たり判定
	Collision m_col;
	// 半径
	float m_radius;

	// 生存判定
	bool m_isExsit;

	// フレーム
	int m_frame;
};

