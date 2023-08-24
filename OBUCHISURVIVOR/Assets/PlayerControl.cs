using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class PlayerControl : MonoBehaviour
{
    public int stageNo = 1;

    // 体力
    int hp = 100;
    int maxHp = 100;

    public GameObject hpObj;
    Slider hpSlider;

    // 強攻撃
    int gaugeCount;
    const int kGaugeMax = 100;

    // 生存確認
    bool isExist;

    // 素材
    int materialNum;

    // 素材数表示
    [SerializeField] GameObject material;
    // 上のTextコンポーネント獲得用
    Text materialTxt;

    void Start()
    {
        hpSlider = hpObj.GetComponent<Slider>();

        isExist = true;

        materialNum = 10;

        materialTxt = material.GetComponent<Text>();

        materialTxt.text = "Material : " + materialNum.ToString();
    }

    void Update()
    {
#if UNITY_EDITOR
        // デバッグ用コード
        if (Input.GetKey(KeyCode.LeftShift))
        {
            // マテリアル数増加
            if (Input.GetKey(KeyCode.M))
            {
                materialNum++;
                materialTxt.text = "Material : " + materialNum.ToString();
            }
        }
#endif
    }

    void FixedUpdate()
    {
        // HPが0未満にならないようにする
        if (hp <= 0)
        {
            hp = 0;
            isExist = false;

            Destroy(this.hpObj);

            // リスタートするときにどのステージ化を確認
            PlayerPrefs.SetInt("StageNo", stageNo);

            // ゲームオーバーシーンの読み込み
            SceneManager.LoadScene("GameOver");
        }
    }

    // HPを減らす処理
    public void HpDown(int attack)
    {
        // 死んでいたら処理を行わない
        if (!isExist) return;

        // 敵の攻撃力分HPを減らす
        hp -= attack;

        hpSlider.value = (float)hp / maxHp;

        // 現在のHPをログに流す
        Debug.Log("[Player]" + this.hp);
    }

    // ゲージをためる処理
    public void GaugeUp()
    {
        // 死んでいたら処理を行わない
        if (!isExist) return;

        // ゲージを増やしていく
        gaugeCount += 2;

        // 最大ゲージまで貯まったらそれ以上いかないようにする
        if (kGaugeMax <= gaugeCount)
        {
            gaugeCount = kGaugeMax;
        }
    }

    // 強攻撃処理
    public void GaugeAttack()
    {
        // 死んでいたら処理を行わない
        if (!isExist) return;

        // ゲージがたまっているか
        if (kGaugeMax <= gaugeCount)
        {
            // 強攻撃のやつ発射
        }
    }

    // 所持素材数を渡す
    public int GetMaterialNum()
    {
        return materialNum;
    }

    // 所持数を増やす
    public void UpMaterialNum(int min, int max)
    {
        materialNum += Random.Range(min, max + 1);

        materialTxt.text = "Material : " + materialNum.ToString();
    }

    // 所持数を減らす
    public void DownMaterialNum(int num)
    {
        materialNum -= num;

        materialTxt.text = "Material : " + materialNum.ToString();
    }
}
