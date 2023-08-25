using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ClearBt : MonoBehaviour
{
    Fade fade;

    bool isStart = false;

    float alpha = 1.0f;

    void Start()
    {
        fade = GetComponent<Fade>();
    }

    void FixedUpdate()
    {
        if (isStart)
        {
            // フェード処理
            alpha -= 0.0625f;

            if (alpha <= 0)
            {
                fade.StartFadeOut("Title");
            }
        }
    }

    public void ReturnTitleBt()
    {
        isStart = true;
    }
}
