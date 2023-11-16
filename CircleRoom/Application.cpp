#include "Application.h"
#include "Scene/SceneManager.h"
#include "Common/Input.h"

#include <DxLib.h>
#include <cassert>

//����͏��߂���Q�[���V�[���ɂ����Ăق�������ꎞ�I�ɃQ�[���V�[���ɔ�Ԃ悤�ɂ���
#include "Scene/GamePlayingScene.h"
#include "Scene/TitleScene.h"


namespace 
{
    // ��ʃT�C�Y
    constexpr int kScreenWidth = 1280;
    constexpr int kScreenHeight = 720;

    // FPS�Œ�l
    constexpr int kFpsFrame = 16667;
}

int MyLoadGraph(const wchar_t* path)
{
    int handle = LoadGraph(path);
    assert(handle != -1);
    return handle;
}

Application::Application()
{
    m_windowSize = Size{kScreenWidth, kScreenHeight};
}

void Application::Terminate()
{
    DxLib_End();
}

bool Application::Init()
{
#ifdef _DEBUG
    ChangeWindowMode(true); // �E�B���h�E���[�h�ɂ��܂�
#endif
    SetGraphMode(m_windowSize.w, m_windowSize.h, 16);
    SetWindowText(L"CircleRoom");
    if (DxLib_Init() == -1)
    {
        return false;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    return true;
}

void Application::Run()
{
    SceneManager manager(this->GetInstance());
    // �ꎞ�I�ɃQ�[���V�[���X�^�[�g��
//    manager.ChangeScene(std::make_shared<TitleScene>(manager));
    manager.ChangeScene(std::make_shared<GamePlayingScene>(manager));

    Input input;
    while (ProcessMessage() != -1)
    {
        // �V�����Q�[�����[�v���n�߂����Ԃ��L��
        m_time = GetNowHiPerformanceCount();

        ClearDrawScreen();
        input.Update(); // ���͂��X�V
        manager.Update(input);
        manager.Draw();
        ScreenFlip();

        // �G�X�P�[�v�L�[�������ꂽ��I������
        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            break;
        }

        while (kFpsFrame > GetNowHiPerformanceCount() - m_time);
    }
    Terminate();
}

const Size& Application::GetWindowSize() const
{
    return m_windowSize;
}
