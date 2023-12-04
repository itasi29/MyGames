#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "Vec2.h"

class Input;
class StageBase;
struct Size;

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

	int GetSlideVolumeX(MoveDir dir) const;
	int GetSlideVolumeY(MoveDir dir) const;

	/// <summary>
	/// クリア情報を保存する
	/// </summary>
	/// <param name="path">パス</param>
	void Save(const std::string& path);

	/// <summary>
	/// クリア情報を読み込む
	/// </summary>
	/// <param name="path">パス</param>
	void Load(const std::wstring& path);


	void SaveClearInf(const std::string& stgName, const std::vector<bool>& inf);

	/// <summary>
	/// クリア情報を持ってくる
	/// </summary>
	/// <param name="stgName">ステージ名</param>
	/// <param name="inf">クリア情報の配列</param>
	/// <returns>true:取得できた / false:取得できなかった</returns>
	bool GetClearInf(const std::string& stgName, std::vector<bool>& inf);

private:
	// ステージのクリア情報群
	std::unordered_map<std::string, std::vector<bool>> m_stageClearTable;

	// ステージのポインタ
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

	void ResetVecX();
	void ResetVecY();

private:
	const Size& m_size;
};

