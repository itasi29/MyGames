using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyPrefab : MonoBehaviour
{
    // 生成する敵キャラを入れる変数
    // 生成間隔
    public GameObject[] enemy;
    public int[] createFrame;
    public int[] farstWaitFrame;

    // 生成する数
    public int[] createEnemyMax;

    // 生成位置
    public int[] createPos;
    const float kBasePosY = 3f;
    public Vector2[] pos = { new Vector2(9.5f, 0) };

    // 生成カウント用
    public int[] createFrameCount;
    // 生成した敵のカウント
    public int[] createCount;
    // 生成フラグ
    public bool[] isCreate;

    void Start()
    {
        for (int i = 0; i < enemy.Length; i++)
        {
            createFrameCount[i] = -farstWaitFrame[i];

            createCount[i] = 0;

            isCreate[i] = true;

            if (createPos[i] == 1)
            {
                pos[i].y = kBasePosY;
            }
            if (createPos[i] == 2)
            {
                pos[i].y = -kBasePosY;
            }
        }
    }

    void FixedUpdate()
    {
        for (int i = 0; i < enemy.Length; i++)
        {
            if (!isCreate[i]) continue;

            createFrameCount[i]++;

            // 生成待機時間が経っているか
            if (createFrame[i] <= createFrameCount[i])
            {
                // 生成数が超えていないか
                if (createCount[i] < createEnemyMax[i])
                {
                    createCount[i]++;

                    createFrameCount[i] = 0;

                    if (createPos[i] == 3)
                    {
                        pos[i].y = kBasePosY * Random.Range(-1, 2);
                    }

                    Instantiate(enemy[i], pos[i], Quaternion.identity);
                }
                else
                {
                    isCreate[i] = false;
                }
            }
        }
    }
}
