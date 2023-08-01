using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FireWaveControl : MonoBehaviour
{
    int attack = 5;

    int CreateFrame = 0;
    const int UpdateFrame = 150;

    void Start()
    {
        this.transform.position = new Vector2(-1.0f, this.transform.position.y);
    }

    void FixedUpdate()
    {
        CreateFrame++;

        if (UpdateFrame <= CreateFrame)
        {
            Destroy(this.gameObject);
        }
    }

    // ぶつかっている間
    void OnTriggerStay2D(Collider2D collision)
    {
        // 敵にあたったらダメージを与える
        if (collision.gameObject.CompareTag("enemy"))
        {
            collision.gameObject.GetComponent<EnemyMove>().SlipDamage(this.attack);
        }
    }
}
