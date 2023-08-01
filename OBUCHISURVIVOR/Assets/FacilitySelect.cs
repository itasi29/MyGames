using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FacilitySelect : MonoBehaviour
{
    [SerializeField] GameObject rightBt;
    [SerializeField] GameObject leftBt;
    [SerializeField] GameObject selectBt;
    [SerializeField] GameObject exisBt;

    [SerializeField] GameObject[] _facility;
    int _selected = 0;

    void Start()
    {
        _selected = PlayerPrefs.GetInt("FacilitySelect", 0);
        _facility[_selected].SetActive(true);
    }

    public void Right()
    {
        // 現在のものを非アクティブ化
        _facility[_selected].SetActive(false);
        _selected++;

        if (_selected >= _facility.Length)
            _selected = 0;
        // 次のものをアクティブ化
        _facility[_selected].SetActive(true);
    }

    public void Left() 
    {
        // 現在のものを非アクティブ化
        _facility[_selected].SetActive(false);
        _selected--;

        if (_selected < 0)
            _selected = _facility.Length - 1;
        // 次のものをアクティブ化
        _facility[_selected].SetActive(true);
    }

    public void SelectFacility()
    {
        PlayerPrefs.SetInt("FacilitySelect", _selected);
        
        this.gameObject.SetActive(false);
        rightBt.SetActive(false);
        leftBt.SetActive(false);
        selectBt.SetActive(false);
        exisBt.SetActive(false);
    }

    public void Exist()
    {
        PlayerPrefs.SetInt("FacilitySelect", _selected);

        this.gameObject.SetActive(false);
        rightBt.SetActive(false);
        leftBt.SetActive(false);
        selectBt.SetActive(false);
        exisBt.SetActive(false);
    }
}
