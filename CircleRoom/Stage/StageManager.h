#pragma once
#include <memory>
#include "Vec2.h"

class Input;
class StageBase;

/// <summary>
/// ステージの遷移をコントロール
/// </summary>
class StageManager
{
public:
	// FIXME:名前は変える
	enum MoveDir
	{
		kDirLeft,
		kDirRight,
		kDirUp,
		kDirDown,
	};

	StageManager();
	~StageManager();

	void Update(Input& input);
	void Draw();

	/// <summary>
	///  ステージの切り替え
	/// </summary>
	/// <param name="nextStage">次のステージ</param>
	void ChangeStage(std::shared_ptr<StageBase> nextStage);

	/// <summary>
	/// ステージを次のとこまでうまく動かすよう
	/// コメント変だから書き直す
	/// </summary>
	/// <param name="pos">動かすベクトル</param>
	/// <param name="handle">画像ハンドル</param>
	void StartMove(MoveDir dir, int handle);

private:
	std::shared_ptr<StageBase> m_stage;

	// 画像を動かしているフレーム
	int m_frame;
	// 画面を動かしているか
	bool m_isStageMove;
	// 画面を動かすベクトル
	Vec2 m_vec;
	// 場所
	Vec2 m_pos;
	// 画面の画像ハンドル
	int m_stageHandle;

	void UpdateMove();
	void DrawMove();
};

