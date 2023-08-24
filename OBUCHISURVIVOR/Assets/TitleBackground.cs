using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TitleBackground : MonoBehaviour
{
    public GameObject backgroundPrefab;
    GameObject bg1;
    GameObject bg2;

    void Start()
    {
        bg1 = Instantiate(backgroundPrefab, new Vector3(0f, 0f, 0f), Quaternion.identity);
        bg2 = Instantiate(backgroundPrefab, new Vector3(-18f, 0f, 0f), Quaternion.identity);
    }

    void FixedUpdate()
    {
        bg1.transform.Translate(0.0625f, 0, 0);
        bg2.transform.Translate(0.0625f, 0, 0);

        if (18f <= bg1.transform.position.x)
        {
            Destroy(bg1);
            bg1 = Instantiate(backgroundPrefab, new Vector3(-18f, 0f, 0f), Quaternion.identity);
        }
        if (18f <= bg2.transform.position.x)
        {
            Destroy(bg2);
            bg2 = Instantiate(backgroundPrefab, new Vector3(-18f, 0f, 0f), Quaternion.identity);
        }
    }
}
