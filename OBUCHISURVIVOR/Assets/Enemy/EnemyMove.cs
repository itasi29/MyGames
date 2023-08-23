using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyMove : MonoBehaviour
{
    // ランダム生成するかしないか
    public bool isRandomCreate = false;

    // 落とす素材数
    public int dropMaterialMin = 0;
    public int dropMaterialMax = 2;

    // 敵のステータス
    public int hp = 25;
    public int attack = 4;
    public float speed = 0.05f;

    // 攻撃のオブジェ
    //   [SerializeField] GameObject attackObj;
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

    // 初期位置位置
    float posX;
    float posY;

    // 止まる位置
    public float stopPosX = -3.0f;

    // 攻撃間隔
    int waitFrameAttack;
    // スリップダメージ間隔
    int waitSlipDamage;
    // スリップダメージの受ける間隔
    const int kSlipDamage = 50;

    // アイス攻撃処理
    int waitFreeze = 0;
    const int kFreeze = 75;
    bool isFreeze;

    // プレイヤーの情報
    PlayerControl player;

    void Start()
    {
        // 位置の初期化
        if (isRandomCreate)
        {
            posX = 9.5f;
            posY = Random.Range(-1, 2) * 3.0f;
            this.transform.position = new Vector2(posX, posY);
        }
        else
        {
            posX = 9.5f;
            posY = this.transform.position.y;
            this.transform.position = new Vector2(posX, posY);
        }

        // 攻撃間隔の初期化
        waitFrameAttack = 0;
        // スリップダメージ間隔の初期化
        waitSlipDamage = kSlipDamage;

        // アイス攻撃処理初期化
        waitFreeze = kFreeze;
        isFreeze = false;

        player = GameObject.Find("PlayerDirector").GetComponent<PlayerControl>();

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
            player.GaugeUp();
            // 素材ドロップ処理
            player.UpMaterialNum(dropMaterialMin, dropMaterialMax);
            Destroy(this.gameObject);
        }

        // アイス攻撃を受けていたら停止
        if (isFreeze)
        {
            if (isSave)
            {
                isSave = false;
                savePower = this.rigid.velocity;
            }

            this.rigid.velocity = Vector2.zero;

            waitFreeze++;
            if (kFreeze <= waitFreeze)
            {
                isFreeze = false;
                isSave = true;
                this.rigid.AddForce(savePower, ForceMode2D.Impulse);
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

                    if (waitFrameAttack > 40)
                    {
                        //        attackInstance = Instantiate(attackObj);
                        //        attackInstance.GetComponent<EnemyAttack>().SetAttack(attack);
                        attackInstance = Instantiate(attackPrefab);
                        attackInstance.transform.position = this.transform.position;

                        // 待機時間を初めに戻す
                        waitFrameAttack = 0;
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

            player.HpDown(attack);
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
