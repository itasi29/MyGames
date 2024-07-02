#include "DebugDraw.h"
#include <DxLib.h>
#include "Common/Game.h"

namespace
{
    // グリッド
    constexpr int kGulidNum = 25;
    constexpr int kGulidSize = 400;
    constexpr int kGulidAdd = kGulidSize * 2 / kGulidNum;
    // 文字情報スタート場所
    constexpr int kDrawStrX = 16;
    constexpr int kDrawStrY = 16;
    // 文字間隔
    constexpr int kDrawStrSpace = 16;
}
using namespace MyEngine;

DebugDraw::DebugDraw()
{
}

DebugDraw::~DebugDraw()
{
}

DebugDraw& DebugDraw::GetInstance()
{
    static DebugDraw instance;
    return instance;
}

void DebugDraw::Gulid() const
{
    for (int x = -kGulidSize; x <= kGulidSize; x += kGulidAdd)
    {
        DrawLine3D(VGet(static_cast<float>(x), 0, -kGulidSize), VGet(static_cast<float>(x), 0, kGulidSize), 0xffff00);
    }
    for (int z = -kGulidSize; z <= kGulidSize; z += kGulidAdd)
    {
        DrawLine3D(VGet(-kGulidSize, 0, static_cast<float>(z)), VGet(kGulidSize, 0, static_cast<float>(z)), 0xff0000);
    }

    // X+-,Z+-の方向が分かりやすいように表示を追加する
    VECTOR dispPos = ConvWorldPosToScreenPos(VGet(200, 0, 0));
    if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
    {
        DrawStringF(dispPos.x, dispPos.y, L"X+", 0xffffff);
    }

    dispPos = ConvWorldPosToScreenPos(VGet(-200, 0, 0));
    if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
    {
        DrawStringF(dispPos.x, dispPos.y, L"X-", 0xffffff);
    }

    dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 200));
    if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
    {
        DrawStringF(dispPos.x, dispPos.y, L"Z+", 0xffffff);
    }

    dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -200));
    if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
    {
        DrawStringF(dispPos.x, dispPos.y, L"Z-", 0xffffff);
    }

}

void DebugDraw::Clear()
{
    m_circleInfo.clear();
    m_lineInfo.clear();
    m_sphereInfo.clear();
}

void DebugDraw::Draw() const
{
    // 円情報描画
    for (const auto& info : m_circleInfo)
    {
        int x = static_cast<int>(info.center.x);
        int y = static_cast<int>(info.center.y);
        int radius = static_cast<int>(info.radius);
        DxLib::DrawCircle(x, y, radius, info.color);
    }
    // 線情報描画
    for (const auto& info : m_lineInfo)
    {
        int startX = static_cast<int>(info.start.x);
        int startY = static_cast<int>(info.start.y);
        int endX = static_cast<int>(info.end.x);
        int endY = static_cast<int>(info.end.y);
        DxLib::DrawLine(startX, startY, endX, endY, info.color);
    }
    // 球描画
    SetUseLighting(false);
    for (const auto& info : m_sphereInfo)
    {
        DxLib::DrawSphere3D(info.center.VGet(), info.radius, info.dirNum, info.color, 0x000000, info.isFill);
    }
    SetUseLighting(true);
}

void DebugDraw::DrawCircle(const CircleInfo& circleInfo)
{
    m_circleInfo.emplace_back(circleInfo);
}

void DebugDraw::DrawLine(const LineInfo& lineInfo)
{
    m_lineInfo.emplace_back(lineInfo);
}

void MyEngine::DebugDraw::DrawSphere(const SphereInfo& sphereInfo)
{
    m_sphereInfo.emplace_back(sphereInfo);
}
