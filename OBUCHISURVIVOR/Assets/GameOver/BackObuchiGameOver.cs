using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class BackObuchiGameOver : MonoBehaviour
{
    public GameObject bg1;
    public GameObject bg2;
    GameObject nowInstance;

    int changeFrame = 0;
    bool isFarst = true;

    AudioSource aud;
    public AudioClip[] boonSe;
    public GameObject boon;
    GameObject boonInstance;

    int boonFrame = 0;
    bool isBoon = false;

    void Start()
    {
        nowInstance = Instantiate(bg1);
        aud = GetComponent<AudioSource>();
    }

    void FixedUpdate()
    {
        changeFrame++;

        if (50 <= changeFrame)
        {
            changeFrame = 0;

            if (isFarst)
            {
                isFarst = false;

                Destroy(nowInstance);

                nowInstance = Instantiate(bg2);
            }
            else
            {
                isFarst = true;

                Destroy(nowInstance);

                nowInstance = Instantiate(bg1);
            }
        }

        boonFrame++;

        if (500 <= boonFrame)
        {
            isBoon = true;

            boonFrame = 0;

            boonInstance = Instantiate(boon);

            aud.PlayOneShot(boonSe[Random.Range(0, boonSe.Length)]);
        }

        if (isBoon)
        {
            boonInstance.transform.Translate(-0.25f, 0f, 0f);

            if (boonInstance.transform.position.x <= -14)
            {
                isBoon = false;

                Destroy(boonInstance);
            }
        }
    }
}
