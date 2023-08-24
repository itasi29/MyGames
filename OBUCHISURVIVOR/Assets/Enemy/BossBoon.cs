using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BossBoon : MonoBehaviour
{
    AudioSource aud;
    int seNo;
    // 攻撃SE
    public AudioClip[] attackSe;

    // 敵のステータス
    int hp = 100;
    int attack = 10;

    // 出現位置、停止位置
    float posX = 5f;

    // 攻撃間隔
    int attackFrame = 150;
    int waitFrameAttack;
    // スリップダメージ間隔
    int waitSlipDamage;
    // スリップダメージの受ける間隔
    const int kSlipDamage = 50;
    Vector2 attackPower = new Vector2(-10f, 0);

    // アイス攻撃処理
    int waitFreeze = 0;
    const int kFreeze = 25;
    bool isFreeze;

    // プレイヤーの情報
    PlayerControl playerInf;
    Rigidbody2D rigid;

    // 2キルボスか確認
    public bool isKill2 = false;

    void Start()
    {
        this.transform.position = new Vector2(posX, 0);

        // 攻撃間隔の初期化
        waitFrameAttack = 0;
        // スリップダメージ間隔の初期化
        waitSlipDamage = kSlipDamage;

        // アイス攻撃処理初期化
        waitFreeze = kFreeze;
        isFreeze = false;

        GameObject player = GameObject.Find("PlayerDirector");
        playerInf = player.GetComponent<PlayerControl>();
        aud = player.GetComponent<AudioSource>();
        rigid  = GetComponent<Rigidbody2D>();
    }

    void FixedUpdate()
    {
        // hpがなくなったら消滅
        if (this.hp <= 0)
        {
            if (isKill2)
            {
                playerInf.LoadClearSceneBoss2();
            }
            else
            {
                playerInf.LoadClearScene();
            }

            Destroy(this.gameObject);
        }

        // アイス攻撃を受けていたら停止
        if (isFreeze)
        {
            waitFreeze++;
            if (kFreeze <= waitFreeze)
            {
                isFreeze = false;
            }
        }
        // 受けていなかったら行動
        else
        {
            waitFrameAttack++;

            if (waitFrameAttack > attackFrame)
            {
                rigid.AddForce(attackPower, ForceMode2D.Impulse);

                // 待機時間を初めに戻す
                waitFrameAttack = 0;

                seNo = Random.Range(0, attackSe.Length);
                aud.PlayOneShot(attackSe[seNo]);
            }
            else if (posX < this.transform.position.x)
            {
                this.rigid.velocity = Vector2.zero;
            }
        }

        // スリップダメージの受けるターンが貯まっていない場合増加
        if (waitSlipDamage <= kSlipDamage)
        {
            waitSlipDamage++;
        }
    }

    void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.CompareTag("barrier"))
        {
            playerInf.HpDown(attack);
        }
    }


    // この敵のHPを減らす
    public void HpDown(int attack)
    {
        hp -= attack;
        // 現在のHPをログに流す
        Debug.Log(this.hp);
    }

    /// アイス攻撃処理
    public void Freeze()
    {
        isFreeze = true;
        waitFreeze = 0;
    }

    /// スリップダメージ処理
    public void SlipDamage(int attack)
    {
        if (kSlipDamage <= waitSlipDamage)
        {
            hp -= attack;
            // 現在のHPをログに流す
            Debug.Log("[Enemy]" + this.hp);

            waitSlipDamage = 0;
        }
    }
}
