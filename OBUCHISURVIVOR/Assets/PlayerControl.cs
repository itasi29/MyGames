using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerControl : MonoBehaviour
{
    Fade fade;
    FacilitySelect facility;

    public int stageNo = 1;

    // 体力
    int hp;
    int maxHp = 300;

    public GameObject hpObj;
    Slider hpSlider;

    // 強攻撃
    int gaugeCount;
    const int kGaugeMax = 100;

    // 生存確認
    bool isExist;

    // 素材
    int materialNum;

    // 回復
    int recoveryMaterialNum = 5;
    int recoveryHp = 1;
    const int kRecoveryFrame = 100;
    int recoveryFrameCount = 0;

    // 攻撃力の増加
    int powerMaterialNum = 2;
    int plusPower = 0;

    // 生成速度短縮
    int createMaterialNum = 2;
    int shortCreate = 0;

    // ボタン
    GameObject canvas;
    GameObject levelUpBt;
    public GameObject recoveryBt;
    public GameObject powerBt;
    public GameObject shortBt;
    public GameObject battenBt;
    public GameObject recoveryTxt;
    GameObject recoveryIst;
    public GameObject powerTxt;
    GameObject powerIst;
    public GameObject shortTxt;
    GameObject shortIst;

    GameObject create0;
    GameObject create1;
    GameObject create2;
    GameObject pouse;

    // 素材数表示
    [SerializeField] GameObject material;
    // 上のTextコンポーネント獲得用
    Text materialTxt;

    // ボス二体を倒したか
    bool isKillBoss = false;

    void Start()
    {
        fade = GetComponent<Fade>();
        facility = GameObject.Find("Facilitys").GetComponent<FacilitySelect>();

        canvas = GameObject.Find("Canvas");

        hpSlider = hpObj.GetComponent<Slider>();

        isExist = true;

        hp = maxHp;

        materialNum = 10;

        materialTxt = material.GetComponent<Text>();

        materialTxt.text = "Material : " + materialNum.ToString();

        levelUpBt = GameObject.Find("LevelUpBt");

        create0 = GameObject.Find("Create0");
        create1 = GameObject.Find("Create1");
        create2 = GameObject.Find("Create2");
        pouse = GameObject.Find("PouseBt");
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
            fade.StartFadeOut("GameOver");
        }

        // 回復処理
        if (hp < maxHp)
        {
            recoveryFrameCount++;
            if (kRecoveryFrame <= recoveryFrameCount)
            {
                recoveryFrameCount = 0;

                hp += recoveryHp;

                if (maxHp < hp)
                {
                    hp = maxHp;
                }

                hpSlider.value = (float)hp / maxHp;
            }
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


    /* ボタン処理 */
    public void LevelUpBt()
    {
        Time.timeScale = 0f;

        levelUpBt.SetActive(false);
        create0.SetActive(false);
        create1.SetActive(false);
        create2.SetActive(false);
        pouse.SetActive(false);

        recoveryBt.SetActive(true);
        recoveryIst = Instantiate(recoveryTxt);
        recoveryIst.transform.SetParent(canvas.transform, false);
        recoveryIst.GetComponent<Text>().text = "必要数 : " + recoveryMaterialNum.ToString();
        powerBt.SetActive(true);
        powerIst = Instantiate(powerTxt);
        powerIst.transform.SetParent(canvas.transform, false);
        powerIst.GetComponent<Text>().text = "必要数 : " + powerMaterialNum.ToString();
        shortBt.SetActive(true);
        shortIst = Instantiate(shortTxt);
        shortIst.transform.SetParent(canvas.transform, false);
        shortIst.GetComponent<Text>().text = "必要数 : " + createMaterialNum.ToString();

        battenBt.SetActive(true);
    }

    // 回復量の増加
    public void UpRecoveryBt()
    {
        if (recoveryMaterialNum <= materialNum)
        {
            recoveryHp++;

            DownMaterialNum(recoveryMaterialNum);

            // レベル上がるごとに＋5
            recoveryMaterialNum += 5;

            recoveryIst.GetComponent<Text>().text = "必要数 : " + recoveryMaterialNum.ToString();
        }
    }

    // 攻撃力の増加
    public void PlusPowerBt()
    {
        if (powerMaterialNum <= materialNum)
        {
            plusPower += 2;

            DownMaterialNum(powerMaterialNum);

            // レベル上がるごとに＋4
            powerMaterialNum += 4;

            if (powerMaterialNum == 42) facility.LevelUp();

            powerIst.GetComponent<Text>().text = "必要数 : " + powerMaterialNum.ToString();
        }
    }

    // 生成速度短縮
    public void ShortCreateTime()
    {
        if (createMaterialNum <= materialNum)
        {
            shortCreate += 5;

            DownMaterialNum(createMaterialNum);

            // レベル上がるごとに＋3
            createMaterialNum += 3;

            if (powerMaterialNum == 32) facility.LevelUp();

            shortIst.GetComponent<Text>().text = "必要数 : " + createMaterialNum.ToString();
        }
    }

    public void BattenBt()
    {
        levelUpBt.SetActive(true);
        create0.SetActive(true);
        create1.SetActive(true);
        create2.SetActive(true);
        pouse.SetActive(true);

        recoveryBt.SetActive(false);
        Destroy(recoveryIst);
        powerBt.SetActive(false);
        Destroy(powerIst);
        shortBt.SetActive(false);
        Destroy(shortIst);

        battenBt.SetActive(false);

        Time.timeScale = 1.0f;
    }

    /* ボタン処理終了 */

    public int  GetPlusPower()
    {
        return plusPower;
    }

    public int GetShortCreateTime()
    {
        return shortCreate;
    }

    // クリアシーンの読み込み
    public void LoadClearScene()
    {
        PlayerPrefs.SetInt("Stage", stageNo + 1);
        fade.StartFadeOut("Clear");
    }

    public void LoadClearSceneBoss2()
    {
        if (isKillBoss)
        {
            PlayerPrefs.SetInt("Stage", stageNo + 1);
            fade.StartFadeOut("Clear");
        }

        isKillBoss = true;
    }
}
