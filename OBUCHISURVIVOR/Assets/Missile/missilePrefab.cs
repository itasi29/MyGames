using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class missilePrefab : MonoBehaviour
{
    public GameObject missile;

    int createFrame;

    Vector3 arrow = new Vector3(0.0f, 0.0f, 1.0f);

    void FixedUpdate()
    {
        createFrame++;

        if (createFrame % 120 == 0)
        {
            Instantiate(this.missile, this.transform.position, Quaternion.AngleAxis(90, arrow));

            createFrame = 0;
        }
    }
}
