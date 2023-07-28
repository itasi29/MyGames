using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RollingControl : MonoBehaviour
{
    int attack = 40;
    Vector2 speed = new Vector2(3.0f, 0.0f);

    Rigidbody2D rigid;

    void Start()
    {
        this.rigid = GetComponent<Rigidbody2D>();
    }

    void FixedUpdate()
    {
        this.rigid.AddForce(speed);

        // 画面外にいったら消す
        if (this.transform.position.x >= 11.5f)
        {
            Destroy(this.gameObject);
        }

        Debug.Log("OK");
        Debug.Log("WWWWWWWWWW");
    }

    void OnTriggerEnter2D(Collider2D collision)
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
