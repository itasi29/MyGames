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
    // �����o�֐��|�C���^�̐錾
    using UpdateFunc_t = void (GameOverScene::*)(Input&);
    using DrawFunc_t = void (GameOverScene::*)();

    // �X�V��Ԃ�\���֐�
    void FadeInUpdate(Input&);
    void NormalUpdate(Input& input);
    void FadeOutUpdate(Input&);

    // �`���Ԃ�\���֐�
    void FadeDraw();
    void NormalDraw();

private:
    UpdateFunc_t m_updateFunc;
    DrawFunc_t m_drawFunc;

    int m_frame;
    int m_textFrame;
    int m_index;
};

