#pragma once
#include "Scene.h"
#include <memory>
#include <vector>

struct Size;
class Player;
class EnemyBase;
class StageManager;

class GamePlayingScene : public Scene
{
public:
    GamePlayingScene(SceneManager& manager);
    ~GamePlayingScene();
    virtual void Update(Input& input);
    virtual void Draw();

private:
    // �����o�֐��|�C���^�̐錾
    using UpdateFunc_t = void (GamePlayingScene::*)(Input&);
    using DrawFunc_t = void (GamePlayingScene::*)();
    UpdateFunc_t m_updateFunc = nullptr;
    DrawFunc_t m_drawFunc = nullptr;

    /* �X�V���� */
    /// <summary>
    /// �t�F�[�h�C������Ƃ��̍X�V����
    /// </summary>
    /// <param name="input">���͏��</param>
    void UpdateFadeIn(Input& input);
    /// <summary>
    /// �t�F�[�h�A�E�g����Ƃ��̍X�V����
    /// </summary>
    /// <param name="input">���͏��</param>
    void UpdateFadeOut(Input& input);
    /// <summary>
    /// �ʏ펞�̍X�V����
    /// </summary>
    /// <param name="input">���͏��</param>
    void UpdateNormal(Input& input);
    /// <summary>
    /// �X�e�[�W�𓮂����Ƃ��̍X�V����
    /// </summary>
    /// <param name="input">���͏��</param>
    void UpdateChangeStage(Input& input);

    /* �`��֐� */
    /// <summary>
    /// �t�F�[�h���̕`��
    /// </summary>
    void DrawFade();
    /// <summary>
    /// �ʏ펞�̕`��
    /// </summary>
    void DrawNormal();
    /// <summary>
    /// �X�e�[�W�𓮂����Ƃ��̕`��
    /// </summary>
    void DrawChangeStage();

    /// <summary>
    /// �ǂ̕`��
    /// </summary>
    void DrawWall();

private:
    // Window�T�C�Y
    const Size& m_windowSize;
    // �t�B�[���h�T�C�Y
    // �Ȃ������ł�2�{�������炿�傤�ǂ̃T�C�Y�ɂȂ�悤�ɂ���
    float m_fieldSize;

    // �X�e�[�W
    std::shared_ptr<StageManager> m_stage;

    // �X�e�[�W�𓮂����Ƃ��̉�ʂ̃n���h��
    int m_screenHandle;

    // �o�߃t���[��
    int m_frame;
};

