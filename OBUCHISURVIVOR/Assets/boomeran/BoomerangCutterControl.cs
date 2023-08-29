using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;
using UnityEngine.Windows.Speech;

public class BoomerangCutterControl : MonoBehaviour
{
    PlayerControl playerInf;

    int attack = 18;

    Vector2 speed = new Vector2(7.0f, 0);
    Rigidbody2D rigid;

    // Z軸の周りを時計回りに毎秒５回転
    Quaternion rot = Quaternion.AngleAxis(-5, Vector3.forward);

    void Start()
    {
        playerInf = GameObject.Find("PlayerDirector").GetComponent<PlayerControl>();

        attack += playerInf.GetPlusPower();

        rigid = GetComponent<Rigidbody2D>();
    }

    void FixedUpdate()
    {
        // 下限値まで速度を下げていく
        if (-3.0f <= this.speed.x)
            speed = new Vector2(this.speed.x - 0.1f, 0);

        // 画面外にいったら消す
        if (this.transform.position.x <= -11.5f)
        {
            Destroy(this.gameObject);
        }

        this.rigid.AddForce(speed);
        // 回転させる
        this.transform.rotation *= rot;
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
