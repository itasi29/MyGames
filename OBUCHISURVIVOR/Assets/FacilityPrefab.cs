using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FacilityPrefab : MonoBehaviour
{
    PlayerControl playerInf;

    // 施設の生成間隔
    public GameObject facility;

    // 生成間隔カウント
    int createFrame = 0;
    // 生成間隔(秒*50)
    public int updateFrame = 300;
    int updateTime;

    bool isCreate = false;

    void Start()
    {
        playerInf = GameObject.Find("PlayerDirector").GetComponent<PlayerControl>();

        createFrame = updateFrame;
    }

    void FixedUpdate()
    {
        if (isCreate)
        {
            createFrame++;


            updateTime = updateFrame - playerInf.GetShortCreateTime();
            if (updateTime <= 50)
            {
                updateTime = 50;
            }

            if (updateTime <= createFrame)
            {
                Instantiate(this.facility, this.transform.position, Quaternion.identity);

                createFrame = 0;
            }
        }
    }

    void OnCollisionEnter2D(Collision2D collision)
    {
        Destroy(this.gameObject);
    }

    public void StartCreate()
    {
        isCreate= true;
    }
}
