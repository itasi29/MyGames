#pragma once
#include "Scene.h"
#include <memory>
#include <vector>

struct Size;
class Player;
class EnemyBase;

class GamePlayingScene : public Scene
{
public:
    GamePlayingScene(SceneManager& manager);
    ~GamePlayingScene();
    virtual void Update(Input& input);
    virtual void Draw();

private:
    // Windowサイズ
    const Size& m_windowSize;
    // フィールドサイズ
    float m_fieldSize;

    // プレイヤー
    std::shared_ptr<Player> m_player;
    // 敵
    std::vector<std::shared_ptr<EnemyBase>> m_enemy;

    // 経過フレーム
    int m_frame = 0;
    // Debug用fps表示
    float m_fps = 0.0f;

    // メンバ関数ポインタの宣言
    using UpdateFunc_t = void (GamePlayingScene::*)(Input&);
    using DrawFunc_t = void (GamePlayingScene::*)();
    UpdateFunc_t m_updateFunc = nullptr;
    DrawFunc_t m_drawFunc = nullptr;

    // 更新関数
    void FadeInUpdate(Input& input);
    void NormalUpdate(Input& input);
    void FadeOutUpdate(Input& input);

    // 描画関数
    void FadeDraw();
    void NormalDraw();
};

