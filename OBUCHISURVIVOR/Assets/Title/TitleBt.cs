using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TitleBt : MonoBehaviour
{
    int stageCheck;

    Fade fade;

    // スタートに使う変数
    // bool isStart = false;

    public GameObject stage1Bt;
    public GameObject stage2Bt;
    public GameObject stage3Bt;

    public GameObject startBt;
    public GameObject guideBt;
    public GameObject quitBt;
    AudioSource aud;

    // 説明ボタンに使う変数
    public GameObject canvas;
    public GameObject battenMark;
    public GameObject[] Guide;
    public GameObject rightBt;
    public GameObject leftBt;
    GameObject nowInstance;
    int guideNo = 0;

    // 終了用に使う変数
    public GameObject checkImg;
    GameObject imgInstance;

    public GameObject yesBt;
    public GameObject noBt;

    bool isStageSelect;

    void Start()
    {
        fade = GameObject.Find("TitleBackground").GetComponent<Fade>();

        aud = GameObject.Find("Main Camera").GetComponent<AudioSource>();

        stageCheck = PlayerPrefs.GetInt("Stage", 1);
    }

    // スタートボタン
    public void StartBt()
    {
        Debug.Log("StartPush");

        isStageSelect = true;

        battenMark.SetActive(true);
        stage1Bt.SetActive(true);

        if (stageCheck >= 2)
        {
            stage2Bt.SetActive(true);
            if (stageCheck >= 3)
            {
                stage3Bt.SetActive(true);
            }
        }

        startBt.SetActive(false);
        guideBt.SetActive(false);
        quitBt.SetActive(false);
    }

    // ステージ１の開始
    public void Stage1Bt()
    {
        fade.StartFadeOut("Stage1");
    }

    // ステージ２の開始
    public void Stage2Bt()
    {
        fade.StartFadeOut("Stage2");
    }

    // ステージ３の開始
    public void Stage3Bt()
    {
        fade.StartFadeOut("Stage3");
    }

    // 説明ボタンに使う関数
    public void GuideBt()
    {
        Debug.Log("GuidePush");

        quitBt.SetActive(false);

        battenMark.SetActive(true);

        rightBt.SetActive(true);
        leftBt.SetActive(true);
        nowInstance = Instantiate(Guide[guideNo]);
        nowInstance.transform.SetParent(canvas.transform, false);
    }

    public void BattenBt()
    {
        if(isStageSelect)
        {
            startBt.SetActive(true);
            guideBt.SetActive(true);

            stage1Bt.SetActive(false);
            if (stageCheck >= 0b01)
            {
                stage2Bt.SetActive(false);
                if (stageCheck >= 0b11)
                {
                    stage3Bt.SetActive(false);
                }
            }
        }
        else
        {
            Destroy(nowInstance);

            rightBt.SetActive(false);
            leftBt.SetActive(false);
        }

        quitBt.SetActive(true);
        battenMark.SetActive(false);
    }

    public void RightBt()
    {
        // 現在のものを削除
        Destroy(nowInstance);

        guideNo++;
        if (1 < guideNo) guideNo = 0;

        // 次のものを生成
        nowInstance = Instantiate(Guide[guideNo]);
        nowInstance.transform.SetParent(canvas.transform, false);
    }

    public void LeftBt()
    {
        // 現在のものを削除
        Destroy(nowInstance);

        guideNo--;
        if (guideNo < 0) guideNo = 1;

        // 次のものを生成
        nowInstance = Instantiate(Guide[guideNo]);
        nowInstance.transform.SetParent(canvas.transform, false);
    }

    // 終了ボタン
    public void QuitBt()
    {
        Debug.Log("QuitPush");

        isStageSelect = false;

        Debug.Log("PUSH");

        imgInstance = Instantiate(checkImg);
        imgInstance.transform.SetParent(canvas.transform, false);
        yesBt.SetActive(true);
        noBt.SetActive(true);

        startBt.SetActive(false);
        guideBt.SetActive(false);
        quitBt.SetActive(false);
    }

    public void YesBt()
    {
        // UNITY_EDITORをプレイしている時(デバック時)は上、
        // ビルド時は下が呼ばれる
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;//ゲームプレイ終了
#else
    Application.Quit();//ゲームプレイ終了
#endif
    }

    public void NoBt()
    {
        Destroy(imgInstance);
        yesBt.SetActive(false);
        noBt.SetActive(false);

        startBt.SetActive(true);
        guideBt.SetActive(true);
        quitBt.SetActive(true);
    }
}
