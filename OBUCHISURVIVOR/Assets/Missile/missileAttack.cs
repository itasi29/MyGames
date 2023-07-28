using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class missileAttack : MonoBehaviour
{
    int attack = 50;

    int waitFrame = 0;

    void FixedUpdate()
    {
        // すべてに反応するように少しの間残す
        waitFrame++;
        if (2 <= waitFrame)
        {
            Destroy(this.gameObject);
        }
    }

    void OnCollisionEnter2D(Collision2D collision)
    {
        // 敵にあたったらダメージを与える
        if (collision.gameObject.CompareTag("enemyShortDistance"))
        {
            collision.gameObject.GetComponent<EnemyMoveShortDistance>().HpDown(this.attack);
        }
        else if (collision.gameObject.CompareTag("enemyLongDistance"))
        {
            collision.gameObject.GetComponent<EnemyMoveLongDistance>().HpDown(this.attack);
        }
    }
}
