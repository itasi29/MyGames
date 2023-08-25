using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class missileAttack : MonoBehaviour
{
    PlayerControl playerInf;

    int attack = 45;

    int waitFrame = 0;

    void Start()
    {
        playerInf = GameObject.Find("PlayerDirector").GetComponent<PlayerControl>();

        this.transform.position = new Vector2(this.transform.position.x + 0.5f, this.transform.position.y);

        attack += playerInf.GetPlusPower();
    }

    void FixedUpdate()
    {
        // すべてに反応するように少しの間残す
        waitFrame++;
        if (2 <= waitFrame)
        {
            Destroy(this.gameObject);
        }
    }

    void OnTriggerEnter2D(Collider2D collision)
    {
        // 敵にあたったらダメージを与える
        if (collision.gameObject.CompareTag("enemy"))
        {
            collision.gameObject.GetComponent<EnemyMove>().HpDown(this.attack);
        }
        else if (collision.gameObject.CompareTag("bossBoon")) 
        {
            collision.gameObject.GetComponent<BossBoon>().HpDown(this.attack);
        }
        else if (collision.gameObject.CompareTag("bossHadouken"))
        {
            collision.gameObject.GetComponent<BossHadouken>().HpDown(this.attack);
        }
    }
}
