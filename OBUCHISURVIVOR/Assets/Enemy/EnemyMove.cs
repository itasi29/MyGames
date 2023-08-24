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
    Rigidbody2D rigid;
    Vector2 attackPower = new Vector2(10f, 0);
    bool isSave = true;
    Vector2 savePower;

    // 止まる位置
    public float stopPosX = -3.0f;

    // 攻撃間隔
    public int attackFrame = 50;
    int waitFrameAttack;
    // スリップダメージ間隔
    int waitSlipDamage;
    // スリップダメージの受ける間隔
    const int kSlipDamage = 50;

    // アイス攻撃処理
    int waitFreeze = 0;
    const int kFreeze = 75;
    bool isFreeze;

    

    void Start()
    {
        GameObject player = GameObject.Find("PlayerDirector");
        playerInf = player.GetComponent<PlayerControl>();
        aud = player.GetComponent<AudioSource>();

        // 攻撃間隔の初期化
        waitFrameAttack = 0;
        // スリップダメージ間隔の初期化
        waitSlipDamage = kSlipDamage;

        // アイス攻撃処理初期化
        waitFreeze = kFreeze;
        isFreeze = false;

        if (isKameka)
        {
            rigid = GetComponent<Rigidbody2D>();
            rigid.AddForce(attackPower, ForceMode2D.Impulse);
        }
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

            Destroy(this.gameObject);
        }

        // アイス攻撃を受けていたら停止
        if (isFreeze)
        {
            if (isKameka)
            {
                if (isSave)
                {
                    isSave = false;
                    savePower = this.rigid.velocity;
                }

                this.rigid.velocity = Vector2.zero;
            }

            waitFreeze++;
            if (kFreeze <= waitFreeze)
            {
                isFreeze = false;
                if (isKameka)
                {
                    isSave = true;
                    this.rigid.AddForce(savePower, ForceMode2D.Impulse);
                }
            }
        }
        // 受けていなかったら行動
        else
        {
            if (isKameka)
            {
                if (isAttack)
                {
                    if (stopPosX < this.transform.position.x)
                    {
                        this.rigid.AddForce(attackPower, ForceMode2D.Impulse);
                        
                        isAttack = false;

                        seNo = Random.Range(0, attackSe.Length);
                        if (seNo != attackSe.Length)
                        {
                            aud.PlayOneShot(attackSe[seNo]);
                        }
                    }
                }
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
            isAttack = true;

            playerInf.HpDown(attack);
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
