using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StormTornadoControl : MonoBehaviour
{
    int attack = 10;

    Rigidbody2D rigid;

    void Start()
    {
        this.rigid = GetComponent<Rigidbody2D>();

        // 初速度のみ与える
        this.rigid.AddForce(new Vector2(7.0f, 0.0f), ForceMode2D.Impulse);
    }

    
    void FixedUpdate()
    {
        // 画面外にいったら消す
        if (this.transform.position.x >= 11.5f)
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
    }
}
