#include "Application.h"
#include "Scene/SceneManager.h"
#include "Stage/StageManager.h"
#include "Common/Input.h"

#include <DxLib.h>
#include <cassert>

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

Application::Application() :
    m_time(0)
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

    // �X�R�[�v�����g���ƈꎞ�I�ɗ�O���������ɂ����Ȃ������ߓ���Ă����A�ǂ��������Ƃ��͂킩��Ȃ�
    {
        SetUseDirect3DVersion(DX_DIRECT3D_9EX);
        SetEnableXAudioFlag(TRUE);
        ChangeFont(L"�l�r �o�S�V�b�N");
    }

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
    {
        StageManager stageManager;
        SceneManager sceneManager(this->GetInstance());
        sceneManager.ChangeScene(std::make_shared<TitleScene>(sceneManager, stageManager));

        Input input;
        while (ProcessMessage() != -1)
        {
            // �V�����Q�[�����[�v���n�߂����Ԃ��L��
            m_time = GetNowHiPerformanceCount();

            ClearDrawScreen();
            input.Update(); // ���͂��X�V
            sceneManager.Update(input);
            sceneManager.Draw();
            ScreenFlip();

            // �G�X�P�[�v�L�[�������ꂽ��I������
            if (CheckHitKey(KEY_INPUT_ESCAPE))
            {
                break;
            }

            while (kFpsFrame > GetNowHiPerformanceCount() - m_time);
        }
    }
    Terminate();
}

const Size& Application::GetWindowSize() const
{
    return m_windowSize;
}
