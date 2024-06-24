#include "DebugDraw.h"
#include <DxLib.h>
#include "Common/Game.h"

namespace
{
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

void DebugDraw::Clear()
{
    m_circleInfo.clear();
    m_lineInfo.clear();
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
}

void DebugDraw::DrawCircle(const CircleInfo& circleInfo)
{
    m_circleInfo.emplace_back(circleInfo);
}

void DebugDraw::DrawLine(const LineInfo& lineInfo)
{
    m_lineInfo.emplace_back(lineInfo);
}
