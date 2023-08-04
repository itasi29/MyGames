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

    // 生成する数
    [SerializeField] int[] createEnemyMax;
    // 生成した敵のカウント
    [SerializeField] int[] createCount;
    // 生成フラグ
    [SerializeField] bool[] isCreate;

    void Start()
    {
        for (int i = 0; i < createFrameCount.Length; i++)
        {
            createFrameCount[i] = 0;
        }

        for (int i = 0; i < createCount.Length; i++)
        {
            createCount[i] = 0;
        }

        for (int i = 0; i < isCreate.Length; i++)
        {
            isCreate[i] = true;
        }
    }

    void FixedUpdate()
    {
        for (int i = 0; i < createFrameCount.Length; i++)
        {
            if (!isCreate[i]) continue;

            createFrameCount[i]++;
        }

        for (int i = 0; i < createFrameCount.Length; i++)
        {
            if (!isCreate[i]) continue;

            // 生成待機時間が経っているか
            if (createFrame[i] <= createFrameCount[i])
            {
                // 生成数が超えていないか
                if (createCount[i] < createEnemyMax[i])
                {
                    createCount[i]++;

                    createFrameCount[i] = 0;

                    Instantiate(enemy[i]);
                }
                else
                {
                    isCreate[i] = false;
                }
            }
        }
    }
}
