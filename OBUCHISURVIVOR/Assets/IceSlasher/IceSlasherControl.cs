using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IceSlasherControl : MonoBehaviour
{
    PlayerControl playerInf;

    int attack = 3;

    float speed = 0.2f;

    int hitNum = 0;

    void Start()
    {
        playerInf = GameObject.Find("PlayerDirector").GetComponent<PlayerControl>();

        attack += playerInf.GetPlusPower();
    }

    void FixedUpdate()
    {
        this.transform.Translate(speed, 0.0f, 0.0f);

        // 画面外にいったら消す
        if (this.transform.position.x >= 11.5f)
        {
            Destroy(this.gameObject);
        }

        if (4 <= hitNum)
        {
            Destroy(this.gameObject);
        }
    }

    void OnTriggerEnter2D(Collider2D collision)
    {
        // 敵にあたったらダメージを与え、アイス攻撃処理を行う
        if (collision.gameObject.CompareTag("enemy"))
        {
            collision.gameObject.GetComponent<EnemyMove>().HpDown(this.attack);
            collision.gameObject.GetComponent<EnemyMove>().Freeze();

            hitNum++;
        }
        else if (collision.gameObject.CompareTag("bossBoon"))
        {
            collision.gameObject.GetComponent<BossBoon>().HpDown(this.attack);
            collision.GetComponent<BossBoon>().Freeze();

            hitNum++;
        }
        else if (collision.gameObject.CompareTag("bossHadouken"))
        {
            collision.gameObject.GetComponent<BossHadouken>().HpDown(this.attack);
            collision.gameObject.GetComponent<BossHadouken>().Freeze();

            hitNum++;
        }
    }
}
