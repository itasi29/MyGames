#pragma once
#include "Scene.h"
class GameOverScene : public Scene
{
public:
    GameOverScene(GameManager& mgr);
    ~GameOverScene();
    virtual void Update(Input& input);
    virtual void Draw();

private:
    // メンバ関数ポインタの宣言
    using UpdateFunc_t = void (GameOverScene::*)(Input&);
    using DrawFunc_t = void (GameOverScene::*)();

    // 更新状態を表す関数
    void FadeInUpdate(Input&);
    void NormalUpdate(Input& input);
    void FadeOutUpdate(Input&);

    // 描画状態を表す関数
    void FadeDraw();
    void NormalDraw();

private:
    UpdateFunc_t m_updateFunc;
    DrawFunc_t m_drawFunc;

    int m_frame;
    int m_textFrame;
    int m_index;
};

