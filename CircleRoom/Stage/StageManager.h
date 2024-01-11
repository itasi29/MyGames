#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "Vec2.h"

class Input;
class StageBase;
struct size;

struct StageData
{
	int bestTime = 0;
	bool isClear = false;
};

enum Ability
{
	kNone,
	kDash,
};

/// <summary>
/// ステージの遷移をコントロール
/// </summary>
class StageManager
{
	// これはダメだけどめんどくさいから簡易実装
public:
	bool m_clear = false;

public:
	// FIXME:名前は変える
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

	// 後で変更or消す
	void DeleteData();



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
	void StartMove(StageManager::StageDir dir, int handle);

	int GetSlideVolumeX(StageDir dir) const;
	int GetSlideVolumeY(StageDir dir) const;

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
	/// そのステージ名のステージデータがない場合作成する
	/// </summary>
	/// <param name="stgName">ステージ名</param>
	void CreateData(const std::string& stgName);

	/// <summary>
	/// クリア情報
	/// </summary>
	/// <param name="stgName">ステージ名</param>
	/// <returns>true:クリア済み / false:未クリア</returns>
	bool IsClearStage(const std::string& stgName);

	/// <summary>
	/// すでにクリアしたことがあるボスか
	/// </summary>
	/// <param name="name">ボスの名前</param>
	/// <returns>true: クリアしている / false:クリアしていない</returns>
	bool IsClearBoss(const std::string& name) const;

	/// <summary>
	/// すでに殺されたことがある敵か
	/// </summary>
	/// <param name="name">敵の名前</param>
	/// <returns>true: 殺されたことがある / false: 殺されたことがない</returns>
	bool IsKilledEnemy(const std::string& name) const;

	/// <summary>
	/// ステージ名に対応するベストクリアタイムを持ってくる
	/// </summary>
	/// <param name="stgName">ステージ名</param>
	/// <returns>ベストタイム</returns>
	int GetBestTime(const std::string& stgName) const;

	/// <summary>
	/// プレイヤーを殺した敵の種類数を返す
	/// </summary>
	/// <returns>種類数</returns>
	int GetEnemyTypeCount() const;

	/// <summary>
	/// アビリティを返す
	/// </summary>
	/// <returns>アビリティ</returns>
	Ability GetAbility() const;

	/// <summary>
	/// クリア情報の保存
	/// クリア済みとする
	/// </summary>
	/// <param name="stgName">ステージ名</param>
	void SaveClear(const std::string& stgName);

	/// <summary>
	/// ボスをクリアしたことがあるとする
	/// </summary>
	/// <param name="name">ボスの名前</param>
	void UpdateClearBoss(const std::string& name);

	/// <summary>
	/// ステージ名に対応するベストタイムの更新
	/// </summary>
	/// <param name="stgName">ステージ名</param>
	/// <param name="bestTime">更新タイム</param>
	void UpdateBestTime(const std::string& stgName, int bestTime);

	/// <summary>
	/// プレイヤーを殺した敵がすでに殺したことがあるかの確認
	/// もし、殺したことがなければ名前を保存し、種類数カウントを増やす
	/// </summary>
	/// <param name="name">敵の名前</param>
	void UpdateEnemyType(const std::string& name);

	/// <summary>
	/// アビリティの変更
	/// </summary>
	/// <param name="ability">アビリティ番号</param>
	void ChangeAbility(Ability ability);

private:
	void UpdateMove();
	void DrawMove();

	void ResetVecX();
	void ResetVecY();

private:
	const size& m_size;

	// ステージのクリア情報群
	std::unordered_map<std::string, StageData> m_stageSaveData;
	// 殺された敵の情報群
	std::vector<std::string> m_killedEnemyNameTable;
	// プレイヤー殺した種類の数
	int m_killedEnemyCount;
	// クリアしたボス情報群
	std::vector<std::string> m_clearBossTable;
	// アビリティ
	Ability m_ability;
	// アビリティの有効無効
	std::unordered_map<Ability, bool> m_abilityActive;

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

};

