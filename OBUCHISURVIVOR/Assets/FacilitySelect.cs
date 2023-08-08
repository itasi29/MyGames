using System.Collections;
using System.Collections.Generic;
using System.Threading.Tasks;
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

    // 元あった施設破壊用
    [SerializeField] GameObject deleteObj;

    // 現状写しているプレハブ
    GameObject nowInstance;
    /*
    // プレハブ生成
    GameObject rightInstance;
    GameObject leftInstance;
    GameObject selectInstance;
    GameObject exisInstance;
    */

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
    // 元あった施設破壊用
    GameObject delete;

    void Start()
    {
        _selected = PlayerPrefs.GetInt("FacilitySelect", 0);

        cmr = Camera.main;
    }

    public void Right()
    {
        // 現在のものを削除
        Destroy(nowInstance);
        _selected++;

        if (_selected >= createFacilitys.Length)
            _selected = 0;
        // 次のものを生成
        nowInstance = Instantiate(createFacilitys[_selected]);
    }

    public void Left()
    {
        // 現在のものを削除
        Destroy(nowInstance);
        _selected--;

        if (_selected < 0)
            _selected = createFacilitys.Length - 1;
        // 次のものを生成
        nowInstance = Instantiate(createFacilitys[_selected]);
    }

    public async void SelectFacility()
    {
        // スクリーン座標をワールド座標に
        setPosition = cmr.ScreenToWorldPoint(setFacilitys[_facilityNo].transform.position);
        // Z軸がカメラ外のため0に
        setPosition.z = 0;

        // 設置する場所に元々施設があったら消す
        delete = Instantiate(deleteObj, setPosition, Quaternion.identity);
        Time.timeScale = 1;
        await Task.Delay(50);
        Time.timeScale = 0;
        Destroy(this.delete);

        // プレハブの生成
        createFacility = Instantiate(createFacilitys[_selected], setPosition, Quaternion.identity);
        createFacility.GetComponent<FacilityPrefab>().StartCreate();
        // ボタンの透明化
        setFacilitys[_facilityNo].GetComponent<Image>().color = color;

        End();
    }

    public void Exist()
    {
        End();
    }

    public void StartInstance()
    {
        nowInstance = Instantiate(createFacilitys[_selected]);
        _facilityNo = PlayerPrefs.GetInt("FacilityNo", 0);

        /*
        rightInstance = Instantiate(rightBt);
        leftInstance = Instantiate(leftBt);
        selectInstance = Instantiate(selectBt);
        exisInstance = Instantiate(exisBt);

        rightInstance.transform.SetParent(rightInstance.transform, false);
        leftInstance.transform.SetParent(leftInstance.transform, false);
        selectInstance.transform.SetParent(selectInstance.transform, false);
        exisInstance.transform.SetParent(exisInstance.transform, false);
        */

        // ほかの動きを止める
        Time.timeScale = 0;
    }

    void End()
    {
        PlayerPrefs.SetInt("FacilitySelect", _selected);

        Destroy(nowInstance);
        rightBt.SetActive(false);
        leftBt.SetActive(false);
        selectBt.SetActive(false);
        exisBt.SetActive(false);

        // ほかを動かしだす
        Time.timeScale = 1;
    }
}
