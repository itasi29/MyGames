#pragma once
#include <DxLib.h>
#include <string>
#include <memory>
#include <list>
#include "Vec2.h"
#include "Utility/Collision.h"

struct size;
class FileBase;

struct PerformanceEff
{
	Vec2 pos;
	int frame = 0;
	bool isEnd = false;
};

/// <summary>
/// ボスの基底クラス
/// </summary>
class BossBase
{
public:
	BossBase(const size& windowSize, float fieldSize, int maxHp);
	virtual ~BossBase();

	void TitleInit();
	void TitleUpdate();
	void TitleDraw();
	virtual void Init(const Vec2& pos, bool isStart = false) = 0;
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
	/// 死亡演出が終了しているか
	/// </summary>
	/// <returns>true : 終了 / false : 演出中</returns>
	bool IsEndPerformance() const { return m_isEndPerformance; }

	/// <summary>
	/// HPを1減らす
	/// </summary>
	virtual bool OnAttack(bool isDash, const Collision& rect);

	virtual void OnDamage() = 0;

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


	/// <summary>
	/// 死亡時の処理
	/// </summary>
	void OnDeath();

protected:
	// メンバ関数ポインタ
	using updateFunc_t = void(BossBase::*)();
	using drawFunc_t = void(BossBase::*)() const;

	virtual void StartUpdate() = 0;
	virtual void NormalUpdate() = 0;
	void HitStopUpdate();
	void DeathUpdate();

	/*死亡演出時の更新関数*/
	void ExplotionUpdate();
	void ShakeUpdate();
	void LastUpdate();

	virtual void StartDraw() const;
	virtual void NormalDraw() const;
	void DeathDraw() const;

	/*死亡演出時の描画関数*/
	void ExplotionDraw() const;
	void ShakeDraw() const;
	void LastDraw() const;

	/// <summary>
	/// HPバーの描画
	/// </summary>
	void DrawHpBar() const;
	void DrawHitWallEffect() const;
	void DrawDamageEffect() const;

protected:
	// HPバーの幅
	const int kHpBarWidth = 270;
	// 角度
	const double kRad = DX_PI / 180;

	// ダメージを受けた際のフレーム
	const int kOnDamageFrame = 33;

	updateFunc_t m_updateFunc;
	drawFunc_t m_drawFunc;
	updateFunc_t m_deathUpdateFunc;
	drawFunc_t m_deathDrawFunc;

	// 実体化するまでの時間
	static const int kApeearFrame = 30;

	double m_angle;

	// スクリーンサイズ
	const size& m_size;
	// フィールドサイズ
	const float m_fieldSize;

	// キャラ画像
	std::shared_ptr<FileBase> m_charImg;
	std::shared_ptr<FileBase> m_shadow;
	// 壁エフェクト
	std::shared_ptr<FileBase> m_wallEffect;
	// ダメージエフェクト
	std::shared_ptr<FileBase> m_damageEffect;
	// HPバー関連の画像
	std::shared_ptr<FileBase> m_hpBar;
	std::shared_ptr<FileBase> m_hpBarBack;
	std::shared_ptr<FileBase> m_hpBarDown;
	std::shared_ptr<FileBase> m_hpBarFrame;

	// 生成時のSE
	std::shared_ptr<FileBase> m_createSe;
	// ダメージを受けた時のSE
	std::shared_ptr<FileBase> m_damageSe;
	// 爆発時のSE
	std::shared_ptr<FileBase> m_explosionSe;
	std::shared_ptr<FileBase> m_explosionLastSe;

	// 敵の名前
	std::string m_name;
	// 色
	unsigned int m_color;
	// 最大体力
	const int m_maxHp;
	// 体力
	int m_hp;
	// 通常のバーの幅
	int m_hpWidth;
	// 赤いバーの幅
	int m_hpDownWidth;

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

	// 死亡演出フラグ
	bool m_isEndPerformance;
	// 演出フレーム
	int m_endPerformanceFrame;
	// 演出回数
	int m_performanceNum;
	// 震わせフラグ
	bool m_isShake;
	// 霧散フラグ
	bool m_isScatter;
	// 爆発エフェクト画像
	std::shared_ptr<FileBase> m_performance;
	// エフェクト描画位置情報
	std::list<PerformanceEff> m_performanceEff;
	// 波紋の半径
	int m_ripple1;
	int m_ripple2;
	int m_ripple3;
	// 半径の色を残す用
	int m_rippleScreen;
};

