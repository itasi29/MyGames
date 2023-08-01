using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyPrefab : MonoBehaviour
{
    // ��������G�L����������ϐ�
    // �����Ԋu
    public GameObject enemy1;
    public int generationFrame1 = 1;
    public GameObject enemy2;
    public int generationFrame2 = 1;
    public GameObject enemy3;
    public int generationrame3 = 1;

    // �����J�E���g�p
    int createFreamCount;

    void Start()
    {
        createFreamCount = 1;
    }

    void FixedUpdate()
    {
        createFreamCount++;

        if (createFreamCount % generationFrame1 == 0 && generationFrame1 != 1)
        {
            Instantiate(enemy1);
        }

        if (createFreamCount % generationFrame2 == 0 && generationFrame2 != 1)
        {
            Instantiate(enemy2);
        }
    }
}
