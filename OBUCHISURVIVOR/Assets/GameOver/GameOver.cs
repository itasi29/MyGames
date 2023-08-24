using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameOver : MonoBehaviour
{
    public GameObject returnGameBt;
    public GameObject returnTitleBt;

    int stageNo;
    bool isStart = false;
    bool isGame = false;

    float alpha = 1.0f;

    void Start()
    {
        stageNo = PlayerPrefs.GetInt("StageNo", 1);
    }

    void FixedUpdate()
    {
        if (isStart)
        {
            // フェード処理
            alpha -= 0.0625f;

            if (alpha <= 0)
            {
                if (isGame)
                {
                    SceneManager.LoadScene("stage" + stageNo.ToString());
                }
                else
                {
                    SceneManager.LoadScene("Title");
                }
            }
        }
    }

    public void ReturnGameBt()
    {
        isStart = true;
        isGame = true;
    }

    public void ReturnTitleBt()
    {
        isStart = true;
    }
}
