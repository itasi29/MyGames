#pragma once
#include "Scene.h"
#include <memory>
#include <vector>

struct m_size;
class Player;
class EnemyBase;

class GamePlayingScene : public Scene
{
public:
    GamePlayingScene(GameManager& mgr);
    ~GamePlayingScene();
    virtual void Update(Input& input);
    virtual void Draw();

private:
    // メンバ関数ポインタの宣言
    using UpdateFunc_t = void (GamePlayingScene::*)(Input&);
    using DrawFunc_t = void (GamePlayingScene::*)();
    UpdateFunc_t m_updateFunc = nullptr;
    DrawFunc_t m_drawFunc = nullptr;

    /* 更新処理 */
    /// <summary>
    /// フェードインするときの更新処理
    /// </summary>
    /// <param name="input">入力情報</param>
    void UpdateFadeIn(Input& input);
    /// <summary>
    /// フェードアウトするときの更新処理
    /// </summary>
    /// <param name="input">入力情報</param>
    void UpdateFadeOut(Input& input);
    /// <summary>
    /// 通常時の更新処理
    /// </summary>
    /// <param name="input">入力情報</param>
    void UpdateNormal(Input& input);

    /* 描画関数 */
    /// <summary>
    /// フェード時の描画
    /// </summary>
    void DrawFade();
    /// <summary>
    /// 通常時の描画
    /// </summary>
    void DrawNormal();

private:
    // フィールドサイズ
    // なおここでは2倍をしたらちょうどのサイズになるようにする
    float m_fieldSize;

    // ステージを動かすときの画面のハンドル
    int m_screenHandle;

    // 経過フレーム
    int m_frame;
};

