#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "Vec2.h"

class Input;
class StageBase;
struct Size;

struct StageData
{
	bool isClear;
	int data;
};

/// <summary>
/// ステージの遷移をコントロール
/// </summary>
class StageManager
{
public:
	// FIXME:名前は変える
	enum MoveDir
	{
		kMoveDirLeft,
		kMoveDirRight,
		kMoveDirUp,
		kMoveDirDown,
	};
	enum StageDir
	{
		kStageLeft,
		kStageRight,
		kStageUp,
		kStageDown,
		kStageMax,
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

	/// <summary>
	/// クリア情報の部分保存
	/// </summary>
	/// <param name="stgName">ステージ名</param>
	/// <param name="inf">クリア情報の配列</param>
	void SaveClearInf(const std::string& stgName, const std::vector<StageData>& inf);

	/// <summary>
	/// クリア情報を持ってくる
	/// </summary>
	/// <param name="stgName">ステージ名</param>
	/// <param name="inf">クリア情報の配列</param>
	/// <returns>true:取得できた / false:取得できなかった</returns>
	bool GetClearInf(const std::string& stgName, std::vector<StageData>& inf);

	/// <summary>
	/// プレイヤーを殺した敵の種類数を返す
	/// </summary>
	/// <returns>種類数</returns>
	int GetKilledEnemyCount() const;

	/// <summary>
	/// プレイヤーを殺した敵がすでに殺したことがあるかの確認
	/// もし、殺したことがなければ名前を保存し、種類数カウントを増やす
	/// </summary>
	/// <param name="name">敵の名前</param>
	void UpdateKilledEnemy(std::string name);

private:
	// ステージのクリア情報群
	std::unordered_map<std::string, std::vector<StageData>> m_stageClearTable;
	// 殺された敵の情報群
	std::vector<std::string> m_killedEnemyNameTable;
	// プレイヤー殺した種類の数
	int m_killedEnemyCount;

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

