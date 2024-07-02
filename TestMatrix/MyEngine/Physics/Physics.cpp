#include "Physics.h"
#include <cassert>
#include "Collidable.h"
#include "ColliderSphere.h"
#include "MyEngine/DebugDraw.h"

using namespace MyEngine;

namespace
{
    // ”»’èÅ‘å‰ñ”
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
    // –¢“o˜^‚È‚ç’Ç‰Á
    if (!isFound)
    {
        m_collidables.emplace_back(collidable);
    }
    // “o˜^Ï‚İ‚È‚ç–³‹
    else
    {
        assert(false);
    }
}

void Physics::Exit(const std::shared_ptr<Collidable>& collidable)
{
    bool isFound = std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end();
    // “o˜^Ï‚İ‚È‚çíœ
    if (isFound)
    {
        m_collidables.remove(collidable);
    }
    // –¢“o˜^‚È‚ç–³‹
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
/// •¨—‚©‚ç‚ÌˆÚ“®‚ğ–¢—ˆ‚ÌÀ•W‚É“K—p
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

                // “–‚½‚Á‚Ä‚¢‚È‚¢ê‡‚ÍŸ‚ÌCollider‚Ö
                if (!IsCollide(objA, objB)) continue;

                // —Dæ“x‚Ìİ’è
                auto primary = objA;
                auto secondary = objB;
                if (objA->m_priority < objB->m_priority)
                {
                    primary = objB;
                    secondary = objA;
                }
                FixNextPos(primary, secondary);

                // –¢’Ç‰Á‚È‚ç’Ç‰Á‚³‚¹‚é
                auto& newParent = m_newCollideInfo[objA.get()];
                if (std::find(newParent.begin(), newParent.end(), objB.get()) == newParent.end())
                {
                    newParent.emplace_back(objB.get());
                }

                // ˆê“x‚Å‚à”»’è‚µ‚½‚çÄ”»’è‚·‚é
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

            // primary‚©‚çsecondary‚Ö‚ÌƒxƒNƒgƒ‹‚ğì¬
            auto primaryToSecondary = secondary->m_rigid.GetNextPos() - primary->m_rigid.GetNextPos();
            // ‚»‚Ì‚Ü‚Ü‚¾‚Æ‚¿‚å‚¤‚Ç“–‚½‚éˆÊ’u‚É‚È‚é‚Ì‚Å­‚µ—]•ª‚É—£‚·
            float  awayDist = primarySphere->radius + secondarySphere->radius + 0.0001f;
            // ’·‚³‚ğ’²®
            primaryToSecondary = primaryToSecondary.GetNormalized() * awayDist;
            // primary‚©‚çƒxƒNƒgƒ‹•ûŒü‚É‰Ÿ‚·
            fixedPos = primary->m_rigid.GetNextPos() + primaryToSecondary;
        }
    }

    secondary->m_rigid.SetNextPos(fixedPos);
}

#if false
void MyEngine::Physics::AddOnCollideInfo(Collidable* objA, Collidable* objB, OnCollideInfoKind kind)
{
    // “o˜^Ï‚İ‚©Šm”F
    bool isEntry = false;
    for (const auto& info : m_onCollideInfo)
    {
        if (info.kind == kind && (info.send == objA || info.send == objB))
        {
            isEntry = true;
            break;
        }
    }
    // –¢“o˜^‚È‚ç“o˜^
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
        // eî•ñ‚Æ‚µ‚Ä“o˜^‚³‚ê‚Ä‚¢‚é‚©
        bool isParentFind = m_preCollideInfo.find(parent.first) != m_preCollideInfo.end();
        
        for (auto& child : parent.second)
        {
            // q‚Æ‚µ‚Ä“o˜^‚³‚ê‚Ä‚¢‚é‚©
            bool isChildFind = false;
            if (isParentFind)
            {
                auto& preInfo = m_preCollideInfo[parent.first];
                isChildFind = std::find(preInfo.begin(), preInfo.end(), child) != preInfo.end();
            }

            // ¡‰ñ“ü‚Á‚Ä‚«‚½ê‡‚ÍEnter‚ğŒÄ‚Ô
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
            // Stay‚Í‚Æ‚è‚ ‚¦‚¸ŒÄ‚Ô
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
        // eî•ñ‚ª“o˜^‚³‚ê‚Ä‚¢‚½‚ç–³‹
        if (m_newCollideInfo.find(parent.first) != m_newCollideInfo.end()) continue;

        for (auto& child : parent.second)
        {
            // q‚ª“o˜^‚³‚ê‚Ä‚¢‚½‚ç–³‹
            auto& newInfo = m_newCollideInfo[parent.first];
            if (std::find(newInfo.begin(), newInfo.end(), child) != newInfo.end()) continue;

            // ¡‰ñ”²‚¯‚Ä‚¢‚Á‚½‚çExist
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
/// ÅI“I‚È–¢—ˆ‚ÌÀ•W‚©‚çŒ»İ‚ÌÀ•W‚É“K—p
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
