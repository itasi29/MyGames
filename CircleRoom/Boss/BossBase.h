#pragma once
#include <string>
#include <memory>
#include "Vec2.h"
#include "Utility/Collision.h"

struct size;
class FileBase;

/// <summary>
/// ボスの基底クラス
/// </summary>
class BossBase
{
public:
	BossBase(const size& windowSize, float fieldSize, int maxHp);
	virtual ~BossBase();

	virtual void Init(const Vec2& pos) = 0;
	void Update();
	void Draw() const;

	/// <summary>
	/// 当たり判定の中心座標を取得
	/// </summary>
	/// <returns>当たり判定の中心座標</returns>
	Collision GetRect() const { return m_col; }

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標の中心座標</returns>
	Vec2 GetPos() const { return m_pos; }

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

	/// <summary>
	/// HPを1減らす
	/// </summary>
	virtual bool OnAttack(bool isDash, const Collision& rect);

protected:
	/// <summary>
	/// 壁に当たったら反射させる
	/// </summary>
	virtual bool Reflection();
	/// <summary>
	/// 反射させる計算
	/// </summary>
	void ReflectionCal(const Vec2& norVec);
	void ShiftReflection(const Vec2& shift);

	/// <summary>
	/// normal関数に変更する
	/// </summary>
	void ChangeNormalFunc();

	/// <summary>
	/// HitStopを開始する
	/// </summary>
	void HitStop();


protected:
	// メンバ関数ポインタ
	using updateFunc_t = void(BossBase::*)();
	using drawFunc_t = void(BossBase::*)() const;

	virtual void StartUpdate() = 0;
	virtual void NormalUpdate() = 0;
	void HitStopUpdate();

	virtual void StartDraw() const;
	virtual void NormalDraw() const;

	/// <summary>
	/// HPバーの描画
	/// </summary>
	void DrawHpBar() const;
	void DrawHitWallEffect() const;

protected:
	updateFunc_t m_updateFunc;
	drawFunc_t m_drawFunc;

	// 実体化するまでの時間
	static const int kApeearFrame = 30;

	// スクリーンサイズ
	const size& m_size;
	// フィールドサイズ
	const float m_fieldSize;

	// キャラ画像
	std::shared_ptr<FileBase> m_charImg;
	// 壁エフェクト
	std::shared_ptr<FileBase> m_wallEffect;
	// ダメージエフェクト
	std::shared_ptr<FileBase> m_damageEffect;

	// 敵の名前
	std::string m_name;
	// 色
	unsigned int m_color;
	// 最大体力
	const int m_maxHp;
	// 体力
	int m_hp;

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

	// HitStopフレーム
	int m_hitStopFrame;

	// 壁に当たったフレーム
	int m_wallHitFrame;

	// 壁に当たった際の場所
	int m_drawWallHitX;
	int m_drawWallHitY;

	// ダメージを受けたフレーム
	int m_onDamagetFrame;

	// ダメージを受けた際の場所
	int m_drawOnDamagetX;
	int m_drawOnDamagetY;

	// 壁エフェクト種類
	int m_lineType;
};

