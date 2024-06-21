#include "Physics.h"
#include <cassert>
#include "Collidable.h"

using namespace MyEngine;

namespace
{
    // ����ő��
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

        for (const auto& objA : m_collidables)
        {
            for (const auto& objB : m_collidables)
            {
                // ����I�u�W�F�̏ꍇ�͎��̃I�u�W�F��
                if (objA == objB) continue;

                // �������Ă��Ȃ��ꍇ�͎��̃I�u�W�F��
                if (!IsCollide(objA, objB)) continue;

                // �D��x�̍������̂̊m�F
                auto primary = objA;
                auto secondary = objB;
                if (objA->GetPriority() < objB->GetPriority())
                {
                    primary = objB;
                    secondary = objA;
                }

                FixNextPos(primary, secondary);
                AddOnCollideInfo(objA, objB);

                // ��x�ł����肵����Ĕ��肷��
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
