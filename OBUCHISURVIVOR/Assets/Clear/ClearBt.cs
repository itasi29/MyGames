using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ClearBt : MonoBehaviour
{
    bool isStart = false;

    float alpha = 1.0f;

    void Start()
    {
        
    }

    void FixedUpdate()
    {
        if (isStart)
        {
            // フェード処理
            alpha -= 0.0625f;

            if (alpha <= 0)
            {
                SceneManager.LoadScene("Title");
            }
        }
    }

    public void ReturnTitleBt()
    {
        isStart = true;
    }
}
