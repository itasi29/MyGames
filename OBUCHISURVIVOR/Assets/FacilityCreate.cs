using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FacilityCreate : MonoBehaviour
{
    [SerializeField] GameObject rightBt;
    [SerializeField] GameObject leftBt;
    [SerializeField] GameObject selectBt;
    [SerializeField] GameObject exisBt;

    public int facilityNo = 0;

    FacilitySelect facility;

    void Start()
    {
        facility = GameObject.Find("Facilitys").GetComponent<FacilitySelect>();
    }

    public void OnClick()
    {
        rightBt.SetActive(true);
        leftBt.SetActive(true);
        selectBt.SetActive(true);
        exisBt.SetActive(true);

        PlayerPrefs.SetInt("FacilityNo", facilityNo);

        facility.StartInstance();
    }

}
