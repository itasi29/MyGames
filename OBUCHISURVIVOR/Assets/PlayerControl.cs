using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerControl : MonoBehaviour
{
    // 体力
    int hp;

    // 強攻撃
    int gaugeCount;
    const int kGaugeMax = 100;

    // 生存確認
    bool isExist;

    // 素材
    int materialNum;

    // 素材数表示
    [SerializeField] GameObject material;
    // 上のTextコンポーネント獲得用
    Text materialTxt;

    void Start()
    {
        isExist = true;

        materialNum = 0;

        materialTxt = material.GetComponent<Text>();

        materialTxt.text = "Material : " + materialNum.ToString();
    }

    void FixedUpdate()
    {
        // HPが0未満にならないようにする
        if (hp <= 0)
        {
            hp = 0;
            isExist = false;
        }
    }

    // HPを減らす処理
    public void HpDown(int attack)
    {
        // 死んでいたら処理を行わない
        if (!isExist) return;

        // 敵の攻撃力分HPを減らす
        hp -= attack;

        // 現在のHPをログに流す
        Debug.Log("[Player]" + this.hp);
    }

    // ゲージをためる処理
    public void GaugeUp()
    {
        // 死んでいたら処理を行わない
        if (!isExist) return;

        // ゲージを増やしていく
        gaugeCount += 2;

        // 最大ゲージまで貯まったらそれ以上いかないようにする
        if (kGaugeMax <= gaugeCount)
        {
            gaugeCount = kGaugeMax;
        }
    }

    // 強攻撃処理
    public void GaugeAttack()
    {
        // 死んでいたら処理を行わない
        if (!isExist) return;

        // ゲージがたまっているか
        if (kGaugeMax <= gaugeCount)
        {
            // 強攻撃のやつ発射
        }
    }

    // 所持素材数を渡す
    public int GetMaterialNum()
    {
        return materialNum;
    }

    // 所持数を増やす
    public void UpMaterialNum(int min, int max)
    {
        materialNum += Random.Range(min, max + 1);

        materialTxt.text = "Material : " + materialNum.ToString();
    }

    // 所持数を減らす
    public void DownMaterialNum(int num)
    {
        materialNum -= num;

        materialTxt.text = "Material : " + materialNum.ToString();
    }
}
