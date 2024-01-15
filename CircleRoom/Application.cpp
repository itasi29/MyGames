#include <DxLib.h>
#include <cassert>

#include "Application.h"
#include "Common/Input.h"
#include "Common/GameManager.h"

#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "FileSystem/FileManager.h"

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
    m_time(0),
    m_isEnd(false)
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

    SetGraphMode(m_size.w, m_size.h, 16);
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
        manager.GetScene()->ChangeScene(std::make_shared<TitleScene>(manager));

        Input input;
        while (ProcessMessage() != -1)
        {
            // �V�����Q�[�����[�v���n�߂����Ԃ��L��
            m_time = GetNowHiPerformanceCount();

            ClearDrawScreen();
            input.Update(); // ���͂��X�V
            manager.GetScene()->Update(input);
            manager.GetScene()->Draw();
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
