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
    // Window�T�C�Y
    const Size& m_windowSize;
    // �t�B�[���h�T�C�Y
    float m_fieldSize;

    // �v���C���[
    std::shared_ptr<Player> m_player;
    // �G
    std::vector<std::shared_ptr<EnemyBase>> m_enemy;

    // �o�߃t���[��
    int m_frame = 0;
    // Debug�pfps�\��
    float m_fps = 0.0f;

    // �����o�֐��|�C���^�̐錾
    using UpdateFunc_t = void (GamePlayingScene::*)(Input&);
    using DrawFunc_t = void (GamePlayingScene::*)();
    UpdateFunc_t m_updateFunc = nullptr;
    DrawFunc_t m_drawFunc = nullptr;

    // �X�V�֐�
    void FadeInUpdate(Input& input);
    void NormalUpdate(Input& input);
    void FadeOutUpdate(Input& input);

    // �`��֐�
    void FadeDraw();
    void NormalDraw();
};

