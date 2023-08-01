using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FacilityCreate : MonoBehaviour
{
    [SerializeField] GameObject rightBt;
    [SerializeField] GameObject leftBt;
    [SerializeField] GameObject selectBt;
    [SerializeField] GameObject exisBt;
    [SerializeField] GameObject facilitys;

    void Start()
    {

    }

    public void OnClickThis()
    {
        Debug.Log("Hit");

        rightBt.SetActive(true);
        leftBt.SetActive(true);
        selectBt.SetActive(true);
        exisBt.SetActive(true);
        facilitys.SetActive(true);
    }

}
