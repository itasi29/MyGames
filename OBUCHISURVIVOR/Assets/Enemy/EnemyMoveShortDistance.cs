using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyMoveShortDistance : MonoBehaviour
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

        player = GameObject.Find("PlayerControl").GetComponent<PlayerControl>();
    }

    void FixedUpdate()
    {
        if (this.hp <= 0)
        {
            player.GaugeUp();
            Destroy(this.gameObject);
        }

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
                player.HpDown(this.attack);

                // 待機時間を初めに戻す
                waitFrameAttack = 0;
            }
        }
    }

    // この敵のHPを減らす
    public void HpDown(int attack)
    {
        hp -= attack;
    }
}
