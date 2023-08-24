using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BackgroundControl : MonoBehaviour
{
    public GameObject backgroundPrefab;
    GameObject bg1;
    GameObject bg2;

    Vector3 pos = new Vector3(-54f, 0f, 0f);

    void Start()
    {
        bg1 = Instantiate(backgroundPrefab, new Vector3(-12f, 0f, 0f), Quaternion.identity);
        bg2 = Instantiate(backgroundPrefab, pos, Quaternion.identity);
    }

    void FixedUpdate()
    {
        bg1.transform.Translate(0.03125f, 0, 0);
        bg2.transform.Translate(0.03125f, 0, 0);

        if (30f <= bg1.transform.position.x)
        {
            Destroy(bg1);
            bg1 = Instantiate(backgroundPrefab, pos, Quaternion.identity);
        }
        if (30f <= bg2.transform.position.x)
        {
            Destroy(bg2);
            bg2 = Instantiate(backgroundPrefab, pos, Quaternion.identity);
        }
    }
}
