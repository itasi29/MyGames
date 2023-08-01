using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyPrefab : MonoBehaviour
{
    // 生成する敵キャラを入れる変数
    // 生成間隔
    [SerializeField] GameObject[] enemy;
    [SerializeField] int[] createFrame;

    // 生成カウント用
    [SerializeField] int[] createFrameCount;

    void Start()
    {
        for (int i = 0; i < createFrameCount.Length; i++)
        {
            createFrameCount[i] = 0;
        }
    }

    void FixedUpdate()
    {
        for (int i = 0; i < createFrameCount.Length; i++)
        {
            createFrameCount[i]++;
        }

        for (int i = 0; i < createFrameCount.Length; i++)
        {
            if (createFrame[i] <= createFrameCount[i])
            {
                createFrameCount[i] = 0;

                Instantiate(enemy[i]);
            }
        }
    }
}
