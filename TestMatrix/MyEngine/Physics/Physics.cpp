#include "Physics.h"
#include <cassert>
#include "Collidable.h"
#include "ColliderSphere.h"

using namespace MyEngine;

namespace
{
    // 判定最大回数
    constexpr int kCheckMaxCount = 1000;
}

Physics::Physics()
{
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

    for (const auto& info : m_onCollideInfo)
    {
        info.func(*info.send);
    }
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

        for (auto itA = m_collidables.begin(); itA != m_collidables.end(); /* 下のループで増やしているはず */)
        {
            for (auto itB = ++itA; itB != m_collidables.end(); ++itB)
            {
                // Aが持っているColliderを全てまわす
                for (auto& colliderA : itA->get()->m_collider)
                {
                    // Bが持っているColliderを全てまわす
                    for (auto& colliderB : itB->get()->m_collider)
                    {
                        // 当たっていない場合は次のColliderへ
                        if (!IsCollide(itA->get(), itB->get(), colliderA, colliderB)) continue;

                        // 優先度の設定
                        auto primary = itA->get();
                        auto secondary = itB->get();
                        auto colliderPrimary = colliderA;
                        auto colliderSecondary = colliderB;
                        if (itA->get()->m_priority < itB->get()->m_priority)
                        {
                            primary = itB->get();
                            secondary = itA->get();
                            colliderPrimary = colliderB;
                            colliderSecondary = colliderA;
                        }
                        FixNextPos(primary, secondary, colliderPrimary, colliderSecondary);

                        // 登録済みか確認
                        bool isEntry = false;
                        for (const auto& info : m_onCollideInfo)
                        {
                            if (info.kind == OnCollideInfoKind::CollideStay && (info.send == primary || info.send == secondary))
                            {
                                isEntry = true;
                                break;
                            }
                        }
                        // 未登録なら登録
                        if (!isEntry)
                        {
                            OnCollideInfo infoA;
                            infoA.send = secondary;
                            infoA.kind = OnCollideInfoKind::CollideStay;
                            infoA.func = [&primary](const Collidable& collider) { primary->OnCollideStay(collider); };
                            m_onCollideInfo.emplace_back(infoA);
                        }
                    }
                }
            }
        }

        //for (const auto& objA : m_collidables)
        //{
        //    for (const auto& objB : m_collidables)
        //    {
        //        // 同一オブジェの場合は次のオブジェへ
        //        if (objA == objB) continue;

        //        // 当たっていない場合は次のオブジェへ
        //        if (!IsCollide(objA, objB)) continue;

        //        // 優先度の高い物体の確認
        //        auto primary = objA;
        //        auto secondary = objB;
        //        if (objA->GetPriority() < objB->GetPriority())
        //        {
        //            primary = objB;
        //            secondary = objA;
        //        }

        //        FixNextPos(primary, secondary);
        //        AddOnCollideInfo(objA, objB);

        //        // 一度でも判定したら再判定する
        //        isCheck = true;
        //        break;
        //    }

        //    if (isCheck) break;
        //}

        if (isCheck && checkCount > kCheckMaxCount)
        {
            
        }
    }
}

bool Physics::IsCollide(const Collidable* objA, const Collidable* objB, const std::shared_ptr<ColliderBase>& colliderA, const std::shared_ptr<ColliderBase>& colliderB) const
{
    bool isCollide = false;

    auto kindA = colliderA->GetKind();
    auto kindB = colliderB->GetKind();

    if (kindA == ColliderBase::Kind::Sphere && kindB == ColliderBase::Kind::Sphere)
    {
        auto sphereA = dynamic_cast<ColliderSphere*>(colliderA.get());
        auto sphereB = dynamic_cast<ColliderSphere*>(colliderB.get());

        auto aToB = objB->m_rigid.GetNextPos() - objA->m_rigid.GetNextPos();
        float sumRadius = sphereA->radius + sphereB->radius;
        isCollide = (aToB.SqLength() < sumRadius * sumRadius);
    }

    return isCollide;
}

void Physics::FixNextPos(const Collidable* primary, const Collidable* secondary, const std::shared_ptr<ColliderBase>& colliderPrimary, const std::shared_ptr<ColliderBase>& colliderSecondary) const
{
    Vec3 fixedPos;

    auto primaryKind = colliderPrimary->GetKind();
    auto secondaryKind = colliderSecondary->GetKind();

    if (primaryKind == ColliderBase::Kind::Sphere)
    {
        if (secondaryKind == ColliderBase::Kind::Sphere)
        {
            auto primarySphere = dynamic_cast<ColliderSphere*>(colliderPrimary.get());
            auto secondarySphere = dynamic_cast<ColliderSphere*>(colliderSecondary.get());

            // primaryからsecondaryへのベクトルを作成
            auto primaryToSecondary = secondary->m_rigid.GetNextPos() - primary->m_rigid.GetNextPos();
            // そのままだとちょうど当たる位置になるので少し余分に離す
            float  awayDist = primarySphere->radius + secondarySphere->radius + 0.0001f;
            // 長さを調整
            primaryToSecondary = primaryToSecondary.GetNormalized() * awayDist;
            // primaryからベクトル方向に押す
            fixedPos = primary->m_rigid.GetNextPos() + primaryToSecondary;
        }
    }
}

void MyEngine::Physics::AddOnCollideInfo(Collidable* objA, Collidable* objB, OnCollideInfoKind kind)
{
    // 登録済みか確認
    bool isEntry = false;
    for (const auto& info : m_onCollideInfo)
    {
        if (info.kind == kind && (info.send == objA || info.send == objB))
        {
            isEntry = true;
            break;
        }
    }
    // 未登録なら登録
    if (!isEntry)
    {
        OnCollideInfo infoA;
        infoA.send = objB;
        infoA.kind = kind;
        infoA.func = GetOnCollideInfoFunc(objA, kind);
        OnCollideInfo infoB;
        infoB.send = objA;
        infoB.kind = kind;
        infoB.func = GetOnCollideInfoFunc(objB, kind);

        m_onCollideInfo.emplace_back(infoA);
        m_onCollideInfo.emplace_back(infoB);
    }
}

std::function<void(const Collidable&)> MyEngine::Physics::GetOnCollideInfoFunc(Collidable* obj, OnCollideInfoKind kind)
{
    std::function<void(const Collidable&)> func;

    if (kind == OnCollideInfoKind::CollideEnter)
    {
        func = [&obj](const Collidable& collider) { obj->OnCollideEnter(collider); };
    }
    else if (kind == OnCollideInfoKind::CollideStay)
    {
        func = [&obj](const Collidable& collider) { obj->OnCollideStay(collider); };
    }
    else if (kind == OnCollideInfoKind::CollideExit)
    {
        func = [&obj](const Collidable& collider) { obj->OnCollideExit(collider); };
    }
    else if (kind == OnCollideInfoKind::TriggerEnter)
    {
        func = [&obj](const Collidable& collider) { obj->OnTriggerEnter(collider); };
    }
    else if (kind == OnCollideInfoKind::TriggerStay)
    {
        func = [&obj](const Collidable& collider) { obj->OnTriggerStay(collider); };
    }
    else if (kind == OnCollideInfoKind::TriggerExit)
    {
        func = [&obj](const Collidable& collider) { obj->OnTriggerExit(collider); };
    }

    return func;
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
