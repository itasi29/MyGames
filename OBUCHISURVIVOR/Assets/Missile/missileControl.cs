using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class missileControl : MonoBehaviour
{
    // 移動速度
    float speed = -0.01f;

    // 攻撃をするようの変数
    public GameObject attack;

    void Start()
    {
        // 横向きに発射させるよう回転
        this.transform.Rotate(0.0f, 0.0f, 90.0f);
    }

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

    void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("enemy"))
        {
            Instantiate(this.attack, this.transform.position, Quaternion.identity);

            Destroy(this.gameObject);
        }
        else if (collision.gameObject.CompareTag("bossBoon"))
        {
            Instantiate(this.attack, this.transform.position, Quaternion.identity);

            Destroy(this.gameObject);
        }
        else if (collision.gameObject.CompareTag("bossHadouken"))
        {
            Instantiate(this.attack, this.transform.position, Quaternion.identity);

            Destroy(this.gameObject);
        }
    }
}
