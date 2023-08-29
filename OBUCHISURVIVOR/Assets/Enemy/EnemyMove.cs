using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyMove : MonoBehaviour
{
    // プレイヤーの情報
    PlayerControl playerInf;

    AudioSource aud;
    int seNo;
    // 攻撃SE
    public AudioClip[] attackSe;
    // 死亡SE
    public AudioClip deathSe1;
    public AudioClip deathSe2;
    public AudioClip deathSe3;
    public AudioClip deathSe4;
    public AudioClip deathSe5;

    // 落とす素材数
    public int dropMaterialMin = 0;
    public int dropMaterialMax = 2;

    // 敵のステータス
    public int hp = 25;
    public int attack = 4;
    public float speed = 0.05f;

    // 攻撃のオブジェ
    // プレハブから攻撃
    public GameObject attackPrefab;
    GameObject attackInstance;
    // カメカーはそのまま攻撃
    public bool isKameka;
    bool isAttack;
    float attackPower = 0.5f;

    // 止まる位置
    public float stopPosX = -3.0f;

    // 攻撃間隔
    public int attackFrame = 50;
    int waitFrameAttack;
    // スリップダメージ間隔
    int waitSlipDamage;
    // スリップダメージの受ける間隔
    const int kSlipDamage = 50;
    bool isSlip = false;
    int slipAttack = 0;

    // アイス攻撃処理
    int waitFreeze = 0;
    const int kFreeze = 75;
    bool isFreeze;

    bool isDamage = false;
    int damageFrame = 0;
    float alpha = 0f;
    Color cDamage = new Color(1f, 0.75f, 0.75f);
    Color cFreeze = new Color(0f, 0.75f, 0.75f);
    SpriteRenderer sprite;

    void Start()
    {
        GameObject player = GameObject.Find("PlayerDirector");
        playerInf = player.GetComponent<PlayerControl>();
        aud = player.GetComponent<AudioSource>();

        // 攻撃間隔の初期化
        waitFrameAttack = 0;
        // スリップダメージ間隔の初期化
        waitSlipDamage = 0;

        // アイス攻撃処理初期化
        waitFreeze = kFreeze;
        isFreeze = false;

        sprite = GetComponent<SpriteRenderer>();
    }

    void FixedUpdate()
    {
        // hpがなくなったら消滅
        if (this.hp <= 0)
        {
            // ゲージアップ処理
            playerInf.GaugeUp();
            // 素材ドロップ処理
            playerInf.UpMaterialNum(dropMaterialMin, dropMaterialMax);

            switch (Random.Range(0, 5))
            {
                case 0:
                    aud.PlayOneShot(deathSe1);
                    break;
                case 1:
                    aud.PlayOneShot(deathSe2);
                    break;
                case 2:
                    aud.PlayOneShot(deathSe3);
                    break;
                case 3:
                    aud.PlayOneShot(deathSe4);
                    break;
                default:
                    aud.PlayOneShot(deathSe5);
                    break;
            }

            // 攻撃を消す
            Destroy(this.attackInstance);

            // 敵の削除
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
            if (isKameka)
            {
                if (stopPosX <= this.transform.position.x) attackPower = 0.5f;

                this.transform.position = new Vector2(this.transform.position.x - attackPower, this.transform.position.y);
            }
            else
            {
                // 攻撃位置にいないなら移動する
                if (this.transform.position.x > stopPosX)
                {
                    this.transform.position = new Vector2(this.transform.position.x - speed, this.transform.position.y);
                }
                // 攻撃に位置にいるなら一定間隔で攻撃する
                else
                {
                    waitFrameAttack++;

                    if (waitFrameAttack > attackFrame)
                    {
                        //        attackInstance = Instantiate(attackObj);
                        //        attackInstance.GetComponent<EnemyAttack>().SetAttack(attack);
                        attackInstance = Instantiate(attackPrefab);
                        attackInstance.transform.position = this.transform.position;

                        // 待機時間を初めに戻す
                        waitFrameAttack = 0;

                        seNo = Random.Range(0, attackSe.Length + 1);
                        if (seNo != attackSe.Length)
                        {
                            aud.PlayOneShot(attackSe[seNo]);
                        }
                        
                    }
                }
            }
        }
    }

    void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.CompareTag("barrier"))
        {
            playerInf.HpDown(attack);

            if (isKameka)
            {
                attackPower = -0.03125f;

                seNo = Random.Range(0, attackSe.Length);
                if (seNo != attackSe.Length)
                {
                    aud.PlayOneShot(attackSe[seNo]);
                }
            }
        }
    }

    void OnTriggerEnter2D(Collider2D collision)
    {
        if (isKameka)
        {
            if (collision.gameObject.CompareTag("attack"))
            {
                Destroy(collision.gameObject);
            }
        }
    }

    // 攻撃力のセット
    public int GetAttack()
    {
        return this.attack;
    }

    // この敵のHPを減らす
    public void HpDown(int attack)
    {
        isDamage = true;
        damageFrame = 0;

        hp -= attack;
        // 現在のHPをログに流す
        Debug.Log("[enemy] : " + this.hp);
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
