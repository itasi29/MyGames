using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RollingPrefab : MonoBehaviour
{
    public GameObject rolling;

    int createFrame;

    Vector3 arrow = new Vector3(0.0f, 0.0f, 1.0f);

    void FixedUpdate()
    {
        createFrame++;

        if (createFrame % 75 == 0)
        {
            Instantiate(this.rolling, this.transform.position, Quaternion.AngleAxis(90, arrow));

            createFrame = 0;
        }
    }
}
