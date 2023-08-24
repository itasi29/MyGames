using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BossHadouken : MonoBehaviour
{
    // プレイヤーの情報
    PlayerControl playerInf;

    AudioSource aud;
    int seNo;
    // 攻撃SE
    public AudioClip[] attackSe;

    // 敵のステータス
    int hp = 25;
    int attack = 4;

    // 攻撃のオブジェ
    //   [SerializeField] GameObject attackObj;
    // プレハブから攻撃
    public GameObject attackPrefab;
    GameObject attackInstance;
    
    // 初期位置位置
    const float kBasePosY = 3f;
    Vector2 pos = new Vector2(7f, 0);

    // 攻撃間隔
    int attackFrame = 150;
    int waitFrameAttack;
    // スリップダメージ間隔
    int waitSlipDamage;
    // スリップダメージの受ける間隔
    const int kSlipDamage = 50;

    // アイス攻撃処理
    int waitFreeze = 0;
    const int kFreeze = 25;
    bool isFreeze;

    // 2キルボスか確認
    public bool isKill2 = false;

    void Start()
    {
        GameObject player = GameObject.Find("PlayerDirector");
        playerInf = player.GetComponent<PlayerControl>();
        aud = player.GetComponent<AudioSource>();

        // 位置の初期化
        pos.y = Random.Range(-1, 2) * kBasePosY;
        this.transform.position = pos;

        // 攻撃間隔の初期化
        waitFrameAttack = 0;
        // スリップダメージ間隔の初期化
        waitSlipDamage = kSlipDamage;

        // アイス攻撃処理初期化
        waitFreeze = kFreeze;
        isFreeze = false;
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
                attackInstance = Instantiate(attackPrefab);
                attackInstance.transform.position = this.transform.position;

                // 待機時間を初めに戻す
                waitFrameAttack = 0;

                seNo = Random.Range(0, attackSe.Length);
                aud.PlayOneShot(attackSe[seNo]);

                // ボスの場所移動
                pos.y = Random.Range(-1, 2) * kBasePosY;
                this.transform.position = pos;
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
