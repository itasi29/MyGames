using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FacilityPrefab : MonoBehaviour
{
    // 施設の生成間隔
    public GameObject facility;

    // 生成間隔カウント
    int createFrame = 0;
    // 生成間隔(秒*50)
    public int updateFrame = 300;

    bool isCreate = false;

    void Start()
    {
        createFrame = updateFrame;
    }

    void FixedUpdate()
    {
        if (isCreate)
        {
            createFrame++;

            if (updateFrame <= createFrame)
            {
                Instantiate(this.facility, this.transform.position, Quaternion.identity);

                createFrame = 0;
            }
        }
    }

    public void StartCreate()
    {
        isCreate= true;
    }
}
