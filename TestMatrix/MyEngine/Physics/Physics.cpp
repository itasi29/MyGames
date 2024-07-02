#include "Physics.h"
#include <cassert>
#include "Collidable.h"
#include "ColliderSphere.h"
#include "MyEngine/DebugDraw.h"

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
    m_preCollideInfo = m_newCollideInfo;
    m_newCollideInfo.clear();
    m_onCollideInfo.clear();

    MoveNextPos();

    CheckCollide();

    AddOnCollideInfo();

    FixPos();

    for (const auto& info : m_onCollideInfo)
    {
        OnCollideInfo(info.own, info.send, info.kind);
    }
}

/// <summary>
/// ��������̈ړ��𖢗��̍��W�ɓK�p
/// </summary>
void MyEngine::Physics::MoveNextPos() const
{
    for (const auto& item : m_collidables)
    {
        auto& rigid = item->m_rigid;

        auto pos = rigid.GetPos();
        auto nextPos = pos + rigid.GetVelocity();

        rigid.SetNextPos(nextPos);

#ifdef _DEBUG
        auto& debug = DebugDraw::GetInstance();

        auto kind = item->m_collider->GetKind();
        if (kind == ColliderBase::Kind::Sphere)
        {
            auto sphereData = dynamic_cast<ColliderSphere*>(item->m_collider.get());
            DebugDraw::SphereInfo preInfo;
            preInfo.center = pos;
            preInfo.radius = sphereData->radius;
            preInfo.color = DebugDraw::kBeforColor;
            debug.DrawSphere(preInfo);
            DebugDraw::SphereInfo newInfo;
            newInfo.center = nextPos;
            newInfo.radius = sphereData->radius;
            newInfo.color = DebugDraw::kNextColor;
            debug.DrawSphere(newInfo);
        }
#endif
    }
}

void MyEngine::Physics::CheckCollide()
{
    bool isCheck = true;
    int checkCount = 0;
    std::unordered_map<Collidable*, std::list<Collidable*>> newCollideInfo;
    while (isCheck)
    {
        isCheck = false;
        ++checkCount;

        for (const auto& objA : m_collidables)
        {
            for (const auto& objB : m_collidables)
            {
                if (objA == objB) continue;

                // �������Ă��Ȃ��ꍇ�͎���Collider��
                if (!IsCollide(objA, objB)) continue;

                // �D��x�̐ݒ�
                auto primary = objA;
                auto secondary = objB;
                if (objA->m_priority < objB->m_priority)
                {
                    primary = objB;
                    secondary = objA;
                }
                FixNextPos(primary, secondary);

                // ���ǉ��Ȃ�ǉ�������
                auto& newParent = m_newCollideInfo[objA.get()];
                if (std::find(newParent.begin(), newParent.end(), objB.get()) == newParent.end())
                {
                    newParent.emplace_back(objB.get());
                }

                // ��x�ł����肵����Ĕ��肷��
                isCheck = true;
                break;
            }
        }

        if (isCheck && checkCount > kCheckMaxCount)
        {
            
        }
    }
}

bool Physics::IsCollide(const std::shared_ptr<Collidable>& objA, const std::shared_ptr<Collidable>& objB) const
{
    bool isCollide = false;

    auto kindA = objA->m_collider->GetKind();
    auto kindB = objB->m_collider->GetKind();

    if (kindA == ColliderBase::Kind::Sphere && kindB == ColliderBase::Kind::Sphere)
    {
        auto sphereA = dynamic_cast<ColliderSphere*>(objA->m_collider.get());
        auto sphereB = dynamic_cast<ColliderSphere*>(objB->m_collider.get());

        auto aToB = objB->m_rigid.GetNextPos() - objA->m_rigid.GetNextPos();
        float sumRadius = sphereA->radius + sphereB->radius;
        isCollide = (aToB.SqLength() < sumRadius * sumRadius);
    }

    return isCollide;
}

void Physics::FixNextPos(const std::shared_ptr<Collidable>& primary, std::shared_ptr<Collidable>& secondary) const
{
    Vec3 fixedPos;

    auto primaryKind = primary->m_collider->GetKind();
    auto secondaryKind = secondary->m_collider->GetKind();

    if (primaryKind == ColliderBase::Kind::Sphere)
    {
        if (secondaryKind == ColliderBase::Kind::Sphere)
        {
            auto primarySphere = dynamic_cast<ColliderSphere*>(primary->m_collider.get());
            auto secondarySphere = dynamic_cast<ColliderSphere*>(secondary->m_collider.get());

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

    secondary->m_rigid.SetNextPos(fixedPos);
}

#if false
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
#endif

void MyEngine::Physics::AddOnCollideInfo()
{
    for (auto& parent : m_newCollideInfo)
    {
        // �e���Ƃ��ēo�^����Ă��邩
        bool isParentFind = m_preCollideInfo.find(parent.first) != m_preCollideInfo.end();
        
        for (auto& child : parent.second)
        {
            // �q�Ƃ��ēo�^����Ă��邩
            bool isChildFind = false;
            if (isParentFind)
            {
                auto& preInfo = m_preCollideInfo[parent.first];
                isChildFind = std::find(preInfo.begin(), preInfo.end(), child) != preInfo.end();
            }

            // ��������Ă����ꍇ��Enter���Ă�
            if (!isChildFind)
            {
                OnCollideInfoData infoA;
                infoA.own = parent.first;
                infoA.send = child;
                infoA.kind = OnCollideInfoKind::CollideEnter;
                OnCollideInfoData infoB;
                infoB.own = child;
                infoB.send = parent.first;
                infoB.kind = OnCollideInfoKind::CollideEnter;
                m_onCollideInfo.emplace_back(infoA);
                m_onCollideInfo.emplace_back(infoB);
            }
            // Stay�͂Ƃ肠�����Ă�
            OnCollideInfoData infoA;
            infoA.own = parent.first;
            infoA.send = child;
            infoA.kind = OnCollideInfoKind::CollideStay;
            OnCollideInfoData infoB;
            infoB.own = child;
            infoB.send = parent.first;
            infoB.kind = OnCollideInfoKind::CollideStay;
            m_onCollideInfo.emplace_back(infoA);
            m_onCollideInfo.emplace_back(infoB);
        }
    }

    for (auto& parent : m_preCollideInfo)
    {
        // �e��񂪓o�^����Ă����疳��
        if (m_newCollideInfo.find(parent.first) != m_newCollideInfo.end()) continue;

        for (auto& child : parent.second)
        {
            // �q���o�^����Ă����疳��
            auto& newInfo = m_newCollideInfo[parent.first];
            if (std::find(newInfo.begin(), newInfo.end(), child) != newInfo.end()) continue;

            // ���񔲂��Ă�������Exist
            OnCollideInfoData infoA;
            infoA.own = parent.first;
            infoA.send = child;
            infoA.kind = OnCollideInfoKind::CollideExit;
            OnCollideInfoData infoB;
            infoB.own = child;
            infoB.send = parent.first;
            infoB.kind = OnCollideInfoKind::CollideExit;
            m_onCollideInfo.emplace_back(infoA);
            m_onCollideInfo.emplace_back(infoB);
        }
    }
}

void MyEngine::Physics::OnCollideInfo(Collidable* own, Collidable* send, OnCollideInfoKind kind)
{
    if (kind == OnCollideInfoKind::CollideEnter)
    {
        own->OnCollideEnter(*send);
    }
    else if (kind == OnCollideInfoKind::CollideStay)
    {
        own->OnCollideStay(*send);
    }
    else if (kind == OnCollideInfoKind::CollideExit)
    {
        own->OnCollideExit(*send);
    }
    else if (kind == OnCollideInfoKind::TriggerEnter)
    {
        own->OnTriggerEnter(*send);
    }
    else if (kind == OnCollideInfoKind::TriggerStay)
    {
        own->OnTriggerStay(*send);
    }
    else if (kind == OnCollideInfoKind::TriggerExit)
    {
        own->OnTriggerExit(*send);
    }
}

/// <summary>
/// �ŏI�I�Ȗ����̍��W���猻�݂̍��W�ɓK�p
/// </summary>
void Physics::FixPos() const
{
    for (const auto& item : m_collidables)
    {
        auto& rigid = item->m_rigid;

        rigid.SetPos(rigid.GetNextPos());

#ifdef _DEBUG
        auto& debug = DebugDraw::GetInstance();
        auto kind = item->m_collider->GetKind();
        if (kind == ColliderBase::Kind::Sphere)
        {
            auto sphereData = dynamic_cast<ColliderSphere*>(item->m_collider.get());
            DebugDraw::SphereInfo info;
            info.center = rigid.GetPos();
            info.radius = sphereData->radius;
            info.color = DebugDraw::kAffterColor;
            debug.DrawSphere(info);
        }
#endif
    }

}
