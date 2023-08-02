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

    public int facilityNo = 0;

    FacilitySelect facility;

    void Start()
    {
        facility = GameObject.Find("Facilitys").GetComponent<FacilitySelect>();
    }

    public void OnClick()
    {
        Debug.Log("Hit");

        rightBt.SetActive(true);
        leftBt.SetActive(true);
        selectBt.SetActive(true);
        exisBt.SetActive(true);
        facilitys.SetActive(true);

        PlayerPrefs.SetInt("FacilityNo", facilityNo);

        facility.StartInstance();
    }

}
