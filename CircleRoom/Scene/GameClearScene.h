#pragma once
#include "Scene.h"

class GameClearScene : public Scene
{
public:
    GameClearScene(GameManager& mgr);
    ~GameClearScene();
    virtual void Update(Input& input);
    virtual void Draw() const;

private:

    // �X�V��Ԃ�\���֐�
    void FadeInUpdate(Input&);
    void NormalUpdate(Input& input);
    void FadeOutUpdate(Input&);

    // �`���Ԃ�\���֐�
    void FadeDraw() const;
    void NormalDraw() const;

    void DrawInf(int index, int drawY, int handle) const;
private:
    // �����o�֐��|�C���^�̐錾
    using UpdateFunc_t = void (GameClearScene::*)(Input&);
    using DrawFunc_t = void (GameClearScene::*)() const;

    UpdateFunc_t m_updateFunc;
    DrawFunc_t m_drawFunc;

    std::shared_ptr<FileBase> m_result;
    std::shared_ptr<FileBase> m_bgm;

    int m_frame;
    int m_textFrame;
    int m_index;
};

