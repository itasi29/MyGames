#pragma once
#include "Scene.h"
#include <memory>
#include <vector>

struct m_size;
class Player;
class EnemyBase;
class FileBase;

class GamePlayingScene : public Scene
{
public:
    GamePlayingScene(GameManager& mgr, Input& input);
    ~GamePlayingScene();
    virtual void Update(Input& input);
    virtual void Draw() const;

private:
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

    /* �`��֐� */
    /// <summary>
    /// �t�F�[�h���̕`��
    /// </summary>
    void DrawFade() const;
    /// <summary>
    /// �ʏ펞�̕`��
    /// </summary>
    void DrawNormal() const;

    void StartStage(Input& input);

private:
    // �����o�֐��|�C���^�̐錾
    using UpdateFunc_t = void (GamePlayingScene::*)(Input&);
    using DrawFunc_t = void (GamePlayingScene::*)() const;

    UpdateFunc_t m_updateFunc;
    DrawFunc_t m_drawFunc;

    // �X�e�[�W�𓮂����Ƃ��̉�ʂ̃n���h��
    int m_screenHandle;

    // �o�߃t���[��
    int m_frame;

    // �X�e�[�W���ő����g���f�[�^
    std::vector<std::shared_ptr<FileBase>> m_stgData;
    // BGM
    std::shared_ptr<FileBase> m_bgm;
};

