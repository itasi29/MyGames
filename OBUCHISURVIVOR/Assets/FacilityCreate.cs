using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FacilityCreate : MonoBehaviour
{
    public int facilityNo = 0;

    FacilitySelect facility;

    void Start()
    {
        facility = GameObject.Find("Facilitys").GetComponent<FacilitySelect>();
    }

    public void OnClick()
    {
        PlayerPrefs.SetInt("FacilityNo", facilityNo);

        facility.StartInstance();
    }

}
