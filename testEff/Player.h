#pragma once
#include <vector>
#include <list>
#include "Vec2.h"

class Input;

struct DirectionVec
{
	Vec2 front;
	Vec2 right;
	Vec2 left;
};

struct EffectData
{
	Vec2 vec;
	Vec2 pos;
	double angle = 0.0;
	int frame = 0;
	bool isEnd = false;
};


class Player
{
public:
	Player();
	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();
	void Update(Input& input);
	void Draw();

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

private:
	// 中心座標
	Vec2 m_pos;
	// 正面方向
	Vec2 m_front;
	// 移動ベクトル
	Vec2 m_vec;
	// 方向ベクトル
	DirectionVec m_dir;

	// 移動エフェクト
	std::list<EffectData> m_effs;

	// 角度
	double m_angle;

	int handle;

	// ダッシュログ
	std::vector<Vec2> m_posLog;
	std::vector<DirectionVec> m_dirLog;
	std::vector<double> m_angleLog;
	// ログフレーム
	int m_logFrame;
	// ダッシュするフレーム
	int m_dashFrame;
	// ダッシュ再利用可能までのフレーム
	int m_dashWaitFrame;
	// ダッシュ判定
	bool m_isDash;
};

