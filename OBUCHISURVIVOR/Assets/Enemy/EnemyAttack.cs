using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyAttack : MonoBehaviour
{
    // 攻撃力
    int attack;

    // どの方向に飛ばすか
    public float powerX = 0;
    public float powerY = 0;

    PlayerControl playerInf;
    public bool isBoss;
    public GameObject enemyInf;


    void Start()
    {
        playerInf = GameObject.Find("PlayerDirector").GetComponent<PlayerControl>();
        GetComponent<Rigidbody2D>().AddForce(new Vector2(powerX, powerY), ForceMode2D.Impulse);

        if (isBoss)
        {
            attack = enemyInf.GetComponent<BossHadouken>().GetAttack();
        }
        else
        {
            attack = enemyInf.GetComponent<EnemyMove>().GetAttack();
        }
    }

    void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("barrier"))
        {
            playerInf.HpDown(attack);

            Destroy(this.gameObject);
        }
    }
}
