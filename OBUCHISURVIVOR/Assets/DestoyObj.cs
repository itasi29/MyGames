using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DestoyObj : MonoBehaviour
{
    static FacilitySelect facility;

    bool isDestroy = false;

    void Start()
    {
        facility = GameObject.Find("Facilitys").GetComponent<FacilitySelect>();

        isDestroy = false;
    }

    void OnCollisionEnter2D(Collision2D collision)
    {
        if (facility.IsCreate() && !isDestroy)
        {
            Destroy(collision.gameObject);

            isDestroy = true;
        }
        Destroy(this.gameObject);
    }
}
