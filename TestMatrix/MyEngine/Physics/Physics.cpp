#include "Physics.h"
#include <cassert>
#include "Collidable.h"

using namespace MyEngine;

namespace
{
    // 判定最大回数
    constexpr int kCheckMaxCount = 1000;
}

Physics& Physics::GetInstance()
{
    static Physics instance;
    return instance;
}

void Physics::Entry(const std::shared_ptr<Collidable>& collidable)
{
    bool isFound = std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end();
    // 未登録なら追加
    if (!isFound)
    {
        m_collidables.emplace_back(collidable);
    }
    // 登録済みなら無視
    else
    {
        assert(false);
    }
}

void Physics::Exit(const std::shared_ptr<Collidable>& collidable)
{
    bool isFound = std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end();
    // 登録済みなら削除
    if (isFound)
    {
        m_collidables.remove(collidable);
    }
    // 未登録なら無視
    else
    {
        assert(false);
    }
}

void Physics::Update()
{
    MoveNextPos();

    CheckCollide();

    FixPos();
}

/// <summary>
/// 物理からの移動を未来の座標に適用
/// </summary>
void MyEngine::Physics::MoveNextPos() const
{
    for (const auto& item : m_collidables)
    {
        auto rigid = item->m_rigid;

        auto pos = rigid.GetPos();
        auto nextPos = pos + rigid.GetVelocity();

        rigid.SetNextPos(nextPos);
    }
}

void MyEngine::Physics::CheckCollide()
{
    bool isCheck = true;
    int checkCount = 0;
    while (isCheck)
    {
        isCheck = false;
        ++checkCount;

        for (const auto& objA : m_collidables)
        {
            for (const auto& objB : m_collidables)
            {
                // 同一オブジェの場合は次のオブジェへ
                if (objA == objB) continue;

                // 当たっていない場合は次のオブジェへ
                if (!IsCollide(objA, objB)) continue;

                // 優先度の高い物体の確認
                auto primary = objA;
                auto secondary = objB;
                if (objA->GetPriority() < objB->GetPriority())
                {
                    primary = objB;
                    secondary = objA;
                }

                FixNextPos(primary, secondary);
                AddOnCollideInfo(objA, objB);

                // 一度でも判定したら再判定する
                isCheck = true;
                break;
            }

            if (isCheck) break;
        }

        if (isCheck && checkCount > kCheckMaxCount)
        {
            
        }
    }
}

bool Physics::IsCollide(const std::shared_ptr<Collidable>& objA, const std::shared_ptr<Collidable>& objB) const
{
    return false;
}

void MyEngine::Physics::FixNextPos(const std::shared_ptr<Collidable>& primary, const std::shared_ptr<Collidable>& secondary) const
{
}

void MyEngine::Physics::AddOnCollideInfo(const std::shared_ptr<Collidable>& objA, const std::shared_ptr<Collidable>& objB)
{
}

/// <summary>
/// 最終的な未来の座標から現在の座標に適用
/// </summary>
void Physics::FixPos() const
{
    for (const auto& item : m_collidables)
    {
        auto rigid = item->m_rigid;

        rigid.SetPos(rigid.GetNextPos());
    }
}
