using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyPrefab : MonoBehaviour
{
    // 生成する敵キャラを入れる変数
    // 生成間隔
    public GameObject[] enemy;
    public int[] createFrame;
    // 生成する数
    public int[] createEnemyMax;

    // 生成位置
    public int[] createPos;
    const float kBasePosY = 3f;
    public Vector2[] pos = { new Vector2(9.5f, 0) };

    // 生成カウント用
    List<int> createFrameCount = new List<int>();
    // 生成した敵のカウント
    List<int> createCount = new List<int>();
    // 生成フラグ
    List<bool> isCreate = new List<bool>();

    void Start()
    {
        for (int i = 0; i < enemy.Length; i++)
        {
            createFrameCount.Add(0);

            createCount.Add(0);

            isCreate.Add(true);

            if (createPos[i] == 0b001)
            {
                pos[i].y = kBasePosY;
            }
            if (createPos[i] == 0b010)
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

                    if (createPos[i] == 0b100)
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
