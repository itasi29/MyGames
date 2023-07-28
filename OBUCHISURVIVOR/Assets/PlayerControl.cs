using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerControl : MonoBehaviour
{
    // ‘Ì—Í
    int hp;

    // ‹­UŒ‚
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

    // HP‚ğŒ¸‚ç‚·ˆ—
    public void HpDown(int attack)
    {
        // “G‚ÌUŒ‚—Í•ªHP‚ğŒ¸‚ç‚·
        hp -= attack;

        // HP‚ª0–¢–‚É‚È‚ç‚È‚¢‚æ‚¤‚É‚·‚é
        if (hp <= 0)
        {
            hp = 0;
        }
    }

    // ƒQ[ƒW‚ğ‚½‚ß‚éˆ—
    public void GaugeUp()
    {
        // ƒQ[ƒW‚ğ‘‚â‚µ‚Ä‚¢‚­
        gaugeCount += 2;

        // Å‘åƒQ[ƒW‚Ü‚Å’™‚Ü‚Á‚½‚ç‚»‚êˆÈã‚¢‚©‚È‚¢‚æ‚¤‚É‚·‚é
        if (kGaugeMax <= gaugeCount)
        {
            gaugeCount = kGaugeMax;
        }
    }
}
