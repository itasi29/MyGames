using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FacilitySelect : MonoBehaviour
{
    // ボタンの情報
    [SerializeField] GameObject rightBt;
    [SerializeField] GameObject leftBt;
    [SerializeField] GameObject selectBt;
    [SerializeField] GameObject exisBt;

    // 入れる場所
    [SerializeField] GameObject[] setFacilitys;
    // 元となるもの
    [SerializeField] GameObject[] createFacilitys;

    // 現状写しているプレハブ
    GameObject nowInstance;

    // 選択している施設の番号
    int _selected = 0;
    // 生成する施設の番号
    int _facilityNo = 0;
    // 完全に透明にする
    Color color = new Color(255f, 255f, 255f, 0.0f);

    // 施設を生成する位置
    Vector3 setPosition;
    // ワールド座標を得るため
    Camera cmr;

    // 生成した施設
    GameObject createFacility;

    void Start()
    {
        _selected = PlayerPrefs.GetInt("FacilitySelect", 0);

        cmr = Camera.main;
    }

    public void Right()
    {
        // 現在のものを非アクティブ化
        Destroy(nowInstance);
        _selected++;

        if (_selected >= createFacilitys.Length)
            _selected = 0;
        // 次のものをアクティブ化
        nowInstance = Instantiate(createFacilitys[_selected]);
    }

    public void Left() 
    {
        // 現在のものを非アクティブ化
        Destroy(nowInstance);
        _selected--;

        if (_selected < 0)
            _selected = createFacilitys.Length - 1;
        // 次のものをアクティブ化
        nowInstance = Instantiate(createFacilitys[_selected]);
    }

    public void SelectFacility()
    {
        PlayerPrefs.SetInt("FacilitySelect", _selected);

        Destroy(nowInstance);
        rightBt.SetActive(false);
        leftBt.SetActive(false);
        selectBt.SetActive(false);
        exisBt.SetActive(false);

        Debug.Log("HIT");

        // スクリーン座標をワールド座標に
        setPosition = cmr.ScreenToWorldPoint(setFacilitys[_facilityNo].transform.position);
        // Z軸がカメラ外のため0に
        setPosition.z = 0;
        // プレハブの生成
        createFacility = Instantiate(createFacilitys[_selected], setPosition, Quaternion.identity);
        createFacility.GetComponent<FacilityPrefab>().StartCreate();
        // ボタンの透明化
        setFacilitys[_facilityNo].GetComponent<Image>().color = color;
    }

    public void Exist()
    {
        PlayerPrefs.SetInt("FacilitySelect", _selected);

        Destroy(nowInstance);
        rightBt.SetActive(false);
        leftBt.SetActive(false);
        selectBt.SetActive(false);
        exisBt.SetActive(false);
    }

    public void StartInstance()
    {
        nowInstance = Instantiate(createFacilitys[_selected]);
        _facilityNo = PlayerPrefs.GetInt("FacilityNo", 0);
    }
}
