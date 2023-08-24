using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RollingControl : MonoBehaviour
{
    int attack = 40;
    Vector2 speed = new Vector2(3.0f, 0.0f);
    Vector3 arrow = new Vector3(0f, 0f, 1f);

    Rigidbody2D rigid;

    void Start()
    {
        this.rigid = GetComponent<Rigidbody2D>();
    }

    void FixedUpdate()
    {
        this.transform.rotation *= Quaternion.AngleAxis(-this.rigid.velocity.x, arrow);

        // 徐々に加速
        this.rigid.AddForce(speed);

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
