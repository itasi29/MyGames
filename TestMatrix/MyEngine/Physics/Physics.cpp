#include "Physics.h"
#include <cassert>
#include "Collidable.h"
#include "ColliderSphere.h"

using namespace MyEngine;

namespace
{
    // ����ő��
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
    // ���o�^�Ȃ�ǉ�
    if (!isFound)
    {
        m_collidables.emplace_back(collidable);
    }
    // �o�^�ς݂Ȃ疳��
    else
    {
        assert(false);
    }
}

void Physics::Exit(const std::shared_ptr<Collidable>& collidable)
{
    bool isFound = std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end();
    // �o�^�ς݂Ȃ�폜
    if (isFound)
    {
        m_collidables.remove(collidable);
    }
    // ���o�^�Ȃ疳��
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
/// ��������̈ړ��𖢗��̍��W�ɓK�p
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

        for (auto itA = m_collidables.begin(); itA != m_collidables.end(); /* ���̃��[�v�ő��₵�Ă���͂� */)
        {
            for (auto itB = ++itA; itB != m_collidables.end(); ++itB)
            {
                // A�������Ă���Collider��S�Ă܂킷
                for (auto& colliderA : itA->get()->m_collider)
                {
                    // B�������Ă���Collider��S�Ă܂킷
                    for (auto& colliderB : itB->get()->m_collider)
                    {
                        // �������Ă��Ȃ��ꍇ�͎���Collider��
                        if (!IsCollide(itA->get(), itB->get(), colliderA, colliderB)) continue;

                        // �D��x�̐ݒ�
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

                        // �o�^�ς݂��m�F
                        bool isEntry = false;
                        for (const auto& info : m_onCollideInfo)
                        {
                            if (info.kind == OnCollideInfoKind::CollideStay && (info.send == primary || info.send == secondary))
                            {
                                isEntry = true;
                                break;
                            }
                        }
                        // ���o�^�Ȃ�o�^
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
        //        // ����I�u�W�F�̏ꍇ�͎��̃I�u�W�F��
        //        if (objA == objB) continue;

        //        // �������Ă��Ȃ��ꍇ�͎��̃I�u�W�F��
        //        if (!IsCollide(objA, objB)) continue;

        //        // �D��x�̍������̂̊m�F
        //        auto primary = objA;
        //        auto secondary = objB;
        //        if (objA->GetPriority() < objB->GetPriority())
        //        {
        //            primary = objB;
        //            secondary = objA;
        //        }

        //        FixNextPos(primary, secondary);
        //        AddOnCollideInfo(objA, objB);

        //        // ��x�ł����肵����Ĕ��肷��
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

            // primary����secondary�ւ̃x�N�g�����쐬
            auto primaryToSecondary = secondary->m_rigid.GetNextPos() - primary->m_rigid.GetNextPos();
            // ���̂܂܂��Ƃ��傤�Ǔ�����ʒu�ɂȂ�̂ŏ����]���ɗ���
            float  awayDist = primarySphere->radius + secondarySphere->radius + 0.0001f;
            // �����𒲐�
            primaryToSecondary = primaryToSecondary.GetNormalized() * awayDist;
            // primary����x�N�g�������ɉ���
            fixedPos = primary->m_rigid.GetNextPos() + primaryToSecondary;
        }
    }
}

void MyEngine::Physics::AddOnCollideInfo(Collidable* objA, Collidable* objB, OnCollideInfoKind kind)
{
    // �o�^�ς݂��m�F
    bool isEntry = false;
    for (const auto& info : m_onCollideInfo)
    {
        if (info.kind == kind && (info.send == objA || info.send == objB))
        {
            isEntry = true;
            break;
        }
    }
    // ���o�^�Ȃ�o�^
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
/// �ŏI�I�Ȗ����̍��W���猻�݂̍��W�ɓK�p
/// </summary>
void Physics::FixPos() const
{
    for (const auto& item : m_collidables)
    {
        auto rigid = item->m_rigid;

        rigid.SetPos(rigid.GetNextPos());
    }
}
