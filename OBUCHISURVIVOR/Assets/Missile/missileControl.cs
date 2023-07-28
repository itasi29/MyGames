using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class missileControl : MonoBehaviour
{
    // 移動速度
    float speed = -0.01f;

    // 攻撃をするようの変数
    public GameObject attack;

    void FixedUpdate()
    {
        this.transform.Translate(0.0f, speed, 0.0f);

        speed -= 0.001f;

        // 画面外にいったら消す
        if (this.transform.position.x >= 11.5f)
        {
            Destroy(this.gameObject);
        }
    }

    void OnCollisionEnter2D(Collision2D collision)
    {
        Instantiate(this.attack, this.transform.position, Quaternion.identity);

        Destroy(this.gameObject);
    }
}
