#pragma once
#include <DxLib.h>
#include <string>
#include <memory>
#include <vector>
#include <list>
#include "Vec2.h"
#include "Collision.h"

struct size;
class FileBase;

struct WallEff
{
	Vec2 pos;
	Vec2 vec;

	double size;
};

struct WallEffMass
{
	std::vector<WallEff> effs;
	bool isUse = true;
	int frame = 0;
};

/// <summary>
/// Enemyクラスの基底
/// </summary>
class EnemyBase
{
public:
	EnemyBase(const size& windowSize, float fieldSize);
	virtual ~EnemyBase();

	void TitleInit();
	void TitleUpdate();
	void TitleDraw() const;
	virtual void Init(const Vec2&, bool isStart = false) {}
	void Update();
	void Draw() const;

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
	// 更新関数
	virtual void StartUpdate() = 0;
	virtual void NormalUpdate() = 0;

	// 描画関数
	virtual void StartDraw() const;
	virtual void NormalDraw() const;

	void DrawHitWallEffect() const;

private:
	void AddWallEff(const Vec2& pos, int sizeX, float shiftX, int sizeY, float shiftY);

protected:
	// 1度を弧度法に
	const double kRad = DX_PI / 180;

	// メンバ関数ポインタ
	using updateFunc_t = void(EnemyBase::*)();
	using drawFunc_t = void(EnemyBase::*)() const;

	updateFunc_t m_updateFunc;
	drawFunc_t m_drawFunc;

	// 実体化するまでの時間
	static const int kApeearFrame = 60;
	// 移動エフェクトのフレーム
	static const int kEffFrame = 10;

	// スクリーンサイズ
	const size& m_size;
	// フィールドのサイズ
	const float m_fieldSize;

	// キャラ画像
	std::shared_ptr<FileBase> m_charImg;
	std::shared_ptr<FileBase> m_shadow;
	// 壁エフェクト
	std::shared_ptr<FileBase> m_wallEffect;

	// 生成時のSE
	std::shared_ptr<FileBase> m_createSe;

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

	double m_angle;

	// フレーム
	int m_frame;

	std::list<WallEffMass> m_wallEff;
};

