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
    int hp = 2000;
    int attack = 18;

    // 出現位置、停止位置
    float posX = 5f;

    // スリップダメージ間隔
    int waitSlipDamage;
    // スリップダメージの受ける間隔
    const int kSlipDamage = 50;
    float attackPower = 0.5f;
    bool isSlip = false;
    int slipAttack = 0;

    // アイス攻撃処理
    int waitFreeze = 0;
    const int kFreeze = 25;
    bool isFreeze;

    // プレイヤーの情報
    PlayerControl playerInf;

    // 2キルボスか確認
    public bool isKill2 = false;

    bool isDamage = false;
    int damageFrame = 0;
    float alpha = 0f;
    Color cDamage = new Color(1f, 0.75f, 0.75f);
    Color cFreeze = new Color(0f, 0.75f, 0.75f);
    SpriteRenderer sprite;

    void Start()
    {
        this.transform.position = new Vector2(posX, 0);

        // スリップダメージ間隔の初期化
        waitSlipDamage = kSlipDamage;

        // アイス攻撃処理初期化
        waitFreeze = kFreeze;
        isFreeze = false;

        GameObject player = GameObject.Find("PlayerDirector");
        playerInf = player.GetComponent<PlayerControl>();
        aud = player.GetComponent<AudioSource>();

        sprite = GetComponent<SpriteRenderer>();
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

        if (isDamage && !isFreeze)
        {
            damageFrame++;

            if (damageFrame % 8 == 0)
            {
                alpha -= 1f;
                if (alpha < 0f) alpha = 1f;

                cDamage.a = alpha;
            }

            if (40 <= damageFrame)
            {
                alpha = 1f;

                cDamage.a = alpha;
                isDamage = false;
            }

            sprite.color = cDamage;
        }

        if (isSlip)
        {
            waitSlipDamage++;


            // スリップダメージの受けるターンが貯まっていない場合増加
            if (kSlipDamage <= waitSlipDamage)
            {
                isSlip = false;
            }

            if (waitSlipDamage % 32 == 0)
            {
                Debug.Log("[FireWave] : attack");
                hp -= slipAttack;

                // 現在のHPをログに流す
                Debug.Log("[FireWave] : enemyHp." + this.hp);

                isDamage = true;
            }
        }

        // アイス攻撃を受けていたら停止
        if (isFreeze)
        {
            waitFreeze++;
            cFreeze.a = cDamage.a;

            sprite.color = cFreeze;

            if (kFreeze <= waitFreeze)
            {
                isFreeze = false;

                if (!isDamage)
                {
                    sprite.color = cDamage;
                }
            }
        }
        // 受けていなかったら行動
        else
        {

            if (posX <= this.transform.position.x) attackPower = 0.5f;

            this.transform.position = new Vector2(this.transform.position.x - attackPower, this.transform.position.y);
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
            attackPower = -0.046875f;

            seNo = Random.Range(0, attackSe.Length);
            if (seNo != attackSe.Length)
            {
                aud.PlayOneShot(attackSe[seNo]);
            }

            playerInf.HpDown(attack);
        }
    }


    // この敵のHPを減らす
    public void HpDown(int attack)
    {
        isDamage = true;
        damageFrame = 0;

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
        Debug.Log("[SlipDamage] : HitEnemy");

        if (!isSlip)
        {
            Debug.Log("[SlipDamage] : DamageStart");

            slipAttack = attack;

            isSlip = true;

            waitSlipDamage = 0;
        }
    }
}
