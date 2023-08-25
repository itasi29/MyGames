using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Fade : MonoBehaviour
{
    GameObject fade;
    Image fadeImage;

    Color c = Color.black;

    string sceneName;
    float alpha = 1.0f;

    bool isIn = true;
    bool isFadeOut = false;

    void Start()
    {
        fade = GameObject.Find("Fade");
        fadeImage = fade.GetComponent<Image>();
    }

    void FixedUpdate()
    {
        if (isIn)
        {
            Debug.Log("[Fade] NowFadeIn");

            alpha -= 0.046875f;

            if (alpha <= 0f)
            {
                isIn = false;

                alpha = 0f;

                fade.SetActive(false);
            }

            c.a = alpha;

            fadeImage.color = c;
        }
        
         if (isFadeOut)
        {
            Debug.Log("[Fade] NowFadeOut");

            alpha += 0.0625f;

            if (1f <= alpha)
            {
                isFadeOut = false;

                alpha = 1f;

                SceneManager.LoadScene(sceneName);
            }

            c.a = alpha;

            fadeImage.color = c;

            Debug.Log("[Fade] alpha = " + alpha);
        }
    }

    void IsFade(string name)
    {
        isFadeOut = true;

        this.sceneName = name;
    }

    public void StartFadeOut(string name)
    {
        Debug.Log("[Fade] FadeOutStart");

        fade.SetActive(true);

        // IsFade(name);

        isFadeOut = true;

        this.sceneName = name;

        Debug.Log("[Fade] PushBt isFadeOut = " + isFadeOut);
    }
}
