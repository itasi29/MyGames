using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerControl : MonoBehaviour
{
    // �̗�
    int hp;

    // ���U��
    int gaugeCount;
    const int kGaugeMax = 100;
    int attack;

    void Start()
    {
        
    }

    void Update()
    {
        if (kGaugeMax <= gaugeCount)
        {
        }
    }

    void FixedUpdate()
    {
        
    }

    // HP�����炷����
    public void HpDown(int attack)
    {
        // �G�̍U���͕�HP�����炷
        hp -= attack;

        // HP��0�����ɂȂ�Ȃ��悤�ɂ���
        if (hp <= 0)
        {
            hp = 0;
        }
    }

    // �Q�[�W�����߂鏈��
    public void GaugeUp()
    {
        // �Q�[�W�𑝂₵�Ă���
        gaugeCount += 2;

        // �ő�Q�[�W�܂Œ��܂����炻��ȏア���Ȃ��悤�ɂ���
        if (kGaugeMax <= gaugeCount)
        {
            gaugeCount = kGaugeMax;
        }
    }
}
