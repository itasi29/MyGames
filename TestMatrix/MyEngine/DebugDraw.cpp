#include "DebugDraw.h"
#include <DxLib.h>
#include "Common/Game.h"

namespace
{
    // •¶šî•ñƒXƒ^[ƒgêŠ
    constexpr int kDrawStrX = 16;
    constexpr int kDrawStrY = 16;
    // •¶šŠÔŠu
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
    // ‰~î•ñ•`‰æ
    for (const auto& info : m_circleInfo)
    {
        int x = static_cast<int>(info.center.x);
        int y = static_cast<int>(info.center.y);
        int radius = static_cast<int>(info.radius);
        DxLib::DrawCircle(x, y, radius, info.color);
    }
    // üî•ñ•`‰æ
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
