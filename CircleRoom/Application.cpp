#include <DxLib.h>
#include <cassert>

#include "Application.h"
#include "Common/Input.h"
#include "Common/GameManager.h"

#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"

#include "Scene/TitleScene.h"

#ifdef _DEBUG
#include "Scene/DebugScene.h"
#endif


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
    m_time(0),
    m_isEnd(false),
#ifdef _DEBUG
    m_isWindows(true)
#else
    m_isWindows(false)
#endif
{
    m_size = size{kScreenWidth, kScreenHeight};
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

    // �E�B���h�E���[�h��ύX�����Ƃ��ɉ摜����������Ȃ��悤�ɂ���
    SetChangeScreenModeGraphicsSystemResetFlag(false);

    // �w�i�F�̎w�� 5f6976
    SetBackgroundColor(0x5f, 0x69, 0x76);

    SetGraphMode(m_size.w, m_size.h, 32);
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
        GameManager& manager = GameManager::GetInstance();
        
        manager.Init();

#ifdef _DEBUG
        manager.GetScene()->ChangeScene(std::make_shared<DebugScene>(manager));
#else
        manager.GetScene()->ChangeScene(std::make_shared<TitleScene>(manager));
#endif

#ifdef _DEBUG
        // ���݃t���[���������Ă��邩�m�F����悤
        int i = 0;
#endif

        Input input;
        while (ProcessMessage() != -1)
        {
            // �V�����Q�[�����[�v���n�߂����Ԃ��L��
            m_time = GetNowHiPerformanceCount();

            i++;

            ClearDrawScreen();
            input.Update(); // ���͂��X�V
            manager.GetScene()->Update(input);
            manager.GetScene()->Draw();

#ifdef _DEBUG
            DrawFormatString(1200, 650, 0xffffff, L"%d", i);
#endif

            ScreenFlip();

            // �I���t���O�������Ă���ΏI������
            if (m_isEnd)
            {
                break;
            }

            while (kFpsFrame > GetNowHiPerformanceCount() - m_time);
        }
    }

    Terminate();
}

void Application::End()
{
    m_isEnd = true;
}

const size& Application::GetWindowSize() const
{
    return m_size;
}

void Application::ChangeWindows()
{
    m_isWindows = !m_isWindows;
    ChangeWindowMode(m_isWindows);
}
