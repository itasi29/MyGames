using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerControl : MonoBehaviour
{
    // 体力
    int hp;

    // 強攻撃
    int gaugeCount;
    const int kGaugeMax = 100;
    int attack;

    // 生存確認
    bool isExist;

    void Start()
    {
        isExist = true;
    }

    void FixedUpdate()
    {
        // 死んだ場合
        if (!isExist)
        {
            
        }
    }

    // HPを減らす処理
    public void HpDown(int attack)
    {
        // 敵の攻撃力分HPを減らす
        hp -= attack;

        // HPが0未満にならないようにする
        if (hp <= 0)
        {
            hp = 0;
        }
    }

    // ゲージをためる処理
    public void GaugeUp()
    {
        // ゲージを増やしていく
        gaugeCount += 2;

        // 最大ゲージまで貯まったらそれ以上いかないようにする
        if (kGaugeMax <= gaugeCount)
        {
            gaugeCount = kGaugeMax;
        }
    }
}
