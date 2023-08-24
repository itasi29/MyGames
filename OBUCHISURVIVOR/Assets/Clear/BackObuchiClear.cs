using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class BackObuchiClear : MonoBehaviour
{
    public GameObject bg1;
    public GameObject bg2;
    GameObject nowInstance;

    int changeFrame = 0;
    bool isFarst = true;

    public GameObject egao;
    public GameObject ikezaki;
    GameObject egaoInstance;
    GameObject ikezakiInstance;

    int moveFrame = 0;
    bool isMove = false;

    void Start()
    {
        nowInstance = Instantiate(bg1);
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

        moveFrame++;

        if (500 <= moveFrame)
        {
            isMove = true;

            moveFrame = 0;

            egaoInstance = Instantiate(egao);
            ikezakiInstance = Instantiate(ikezaki);
        }

        if (isMove)
        {
            egaoInstance.transform.Translate(0f, 0.25f, 0f);
            ikezakiInstance.transform.Translate(0f, 0.25f, 0f);

            if (egaoInstance.transform.position.y <= -7.5)
            {
                Destroy(egaoInstance);
                Destroy(ikezakiInstance);

                isMove = false;
            }
        }
            
    }
}
