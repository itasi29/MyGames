using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyMove : MonoBehaviour
{
    // 敵のステータス
    public int hp = 25;
    public int attack = 4;
    public float speed = 0.05f;

    // スピード倍率
    public int speedMultiplier = 1;

    // 初期位置位置
    float posX;
    float posY;

    // 止まる位置
    float stopPosX = -3.0f;

    // 攻撃間隔
    int waitFrameAttack;
    // スリップダメージ間隔
    int waitSlipDamage;
    // スリップダメージの受ける間隔
    const int kSlipDamage = 50;

    // アイス攻撃処理
    int waitFreeze = 0;
    const int kFreeze = 75;
    bool isFreeze;

    // プレイヤーの情報
    PlayerControl player;

    void Start()
    {
        // 速度の倍率処理
        speed *= speedMultiplier;

        // 位置の初期化
        posY = Random.Range(-1, 2) * 3.0f;
        posX = 9.5f;
        this.transform.position = new Vector2(posX, posY);

        // 攻撃間隔の初期化
        waitFrameAttack = 0;
        // スリップダメージ間隔の初期化
        waitSlipDamage = kSlipDamage;

        // アイス攻撃処理初期化
        waitFreeze = kFreeze;
        isFreeze = false;

        //player = GameObject.Find("PlayerControl").GetComponent<PlayerControl>();
    }

    void FixedUpdate()
    {
        if (this.hp <= 0)
        {
            player.GaugeUp();
            Destroy(this.gameObject);
        }

        // アイス攻撃を受けていたら
        if (isFreeze)
        {
            waitFreeze++;
            if (kFreeze <= waitFreeze)
            {
                isFreeze = false;
            }
        }
        // 受けていなかったら行動
        else
        {
            // 攻撃位置にいないなら移動する
            if (this.transform.position.x > stopPosX)
            {
                this.transform.position = new Vector2(this.transform.position.x - speed, this.transform.position.y);
            }
            // 攻撃に位置にいるなら一定間隔で攻撃する
            else
            {
                // どちらのフレームを増やしていくか判定
                waitFrameAttack++;

                if (waitFrameAttack > 40)
                {
                    // プレイヤーのHPを減らす
                    //player.HpDown(this.attack);

                    // 待機時間を初めに戻す
                    waitFrameAttack = 0;
                }
            }
        }

        // スリップダメージの受けるターンが貯まっていない場合増加
        if (waitSlipDamage <= kSlipDamage)
        {
            waitSlipDamage++;
        }
    }

    // この敵のHPを減らす
    public void HpDown(int attack)
    {
        hp -= attack;
        // 現在のHPをログに流す
        Debug.Log(this.hp);
    }

    // アイス攻撃処理
    public void Freeze()
    {
        isFreeze = true;
        waitFreeze = 0;
    }

    // スリップダメージ処理
    public void SlipDamage(int attack)
    {
        if (kSlipDamage <= waitSlipDamage)
        {
            hp -= attack;
            // 現在のHPをログに流す
            Debug.Log(this.hp);

            waitSlipDamage = 0;
        }
    }
}
