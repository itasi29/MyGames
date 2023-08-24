using System.Collections;
using System.Collections.Generic;
using System.Threading.Tasks;
using UnityEngine;
using UnityEngine.UI;

public class FacilitySelect : MonoBehaviour
{
    GameObject canvas;

    // ボタンの情報
    public GameObject rightBt;
    public GameObject leftBt;
    public GameObject selectBt;
    public GameObject exisBt;

    // レベルアップ処理ボタン
    public GameObject changeBt;
    public GameObject levelUpBt;
    public PlayerControl player;
    public GameObject levelUpImg;

    GameObject materialShortageInstance;

    // 入れる場所
    public GameObject[] setFacilitys = new GameObject[3];
    // 元となるもの
    public GameObject[] createFacilitys = new GameObject[12];
    // 施設名
    public GameObject facilityNameTxt;
    GameObject nameInstance;
    string[] facilityName = new string[6] { "ブーメラン", "ファイアーウェーブ" , "アイススラッシャー", "ミサイル", "ローリング", "ストーム" };

    // 元あった施設破壊用
    public GameObject deleteObj;

    // 施設を建てているかの確認
    bool[] isCreate = new bool[3] { false, false, false };

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
    // 元あった施設破壊用
    GameObject delete;

    int needMaterialNum = 5;

    // 施設変更
    bool[] isProduct = new bool[3];
    bool[] isLevelUp = new bool[3];

    void Start()
    {
        canvas = GameObject.Find("Canvas");

        cmr = Camera.main;
    }

    public void Right()
    {
        // 現在のものを削除
        Destroy(nowInstance);
        _selected++;

        if (!isLevelUp[_facilityNo])
        {
            if (6 <= _selected)
                _selected = 0;

            nameInstance.GetComponent<Text>().text = facilityName[_selected];
        }
        else
        {
            if (12 <= _selected)
                _selected = 6;

            nameInstance.GetComponent<Text>().text = facilityName[_selected - 6];
        }

        // 次のものを生成
        nowInstance = Instantiate(createFacilitys[_selected]);
    }

    public void Left()
    {
        // 現在のものを削除
        Destroy(nowInstance);
        _selected--;

        if (!isLevelUp[_facilityNo])
        {
            if (_selected < 0)
                _selected = 5;

            nameInstance.GetComponent<Text>().text = facilityName[_selected];
        }
        else
        {
            if (_selected < 6)
                _selected = 11;

            nameInstance.GetComponent<Text>().text = facilityName[_selected - 6];
        }

        // 次のものを生成
        nowInstance = Instantiate(createFacilitys[_selected]);
    }

    public async void SelectFacility()
    {
        if (needMaterialNum <= player.GetMaterialNum())
        {
            player.DownMaterialNum(needMaterialNum);

            // 作っていることにする
            isCreate[_facilityNo] = true;
            isProduct[_facilityNo] = true;

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
        else
        {
            MaterialShortage();
        }
    }

    public void Exist()
    {
        End();
    }

    public void StartInstance()
    {
        // ほかの動きを止める
        Time.timeScale = 0;

        _facilityNo = PlayerPrefs.GetInt("FacilityNo", 0);

        if (_facilityNo == 0)
        {
            _selected = PlayerPrefs.GetInt("FacilitySelect1", 0);
        }
        else if (_facilityNo == 1)
        {
            _selected = PlayerPrefs.GetInt("FacilitySelect2", 0);
        }
        else
        {
            _selected = PlayerPrefs.GetInt("FacilitySelect3", 0);
        }

        nowInstance = Instantiate(createFacilitys[_selected]);
        nameInstance = Instantiate(facilityNameTxt);
        nameInstance.transform.SetParent(canvas.transform, false);

        exisBt.SetActive(true);

        if (isLevelUp[_facilityNo])
        {
            rightBt.SetActive(true);
            leftBt.SetActive(true);
            selectBt.SetActive(true);

            _selected += 6;

            needMaterialNum = 10;

            nameInstance.GetComponent<Text>().text = facilityName[_selected - 6];
        }
        else
        {
            if (isCreate[_facilityNo])
            {
                // レベルアップ処理を書く
                changeBt.SetActive(true);
                levelUpBt.SetActive(true);
            }
            else
            {
                rightBt.SetActive(true);
                leftBt.SetActive(true);
                selectBt.SetActive(true);
            }

            nameInstance.GetComponent<Text>().text = facilityName[_selected];
        }
    }

    public void ChangeSystem()
    {
        if (isProduct[_facilityNo])
        {
            levelUpBt.SetActive(false);

            rightBt.SetActive(true);
            leftBt.SetActive(true);
            selectBt.SetActive(true);

            isProduct[_facilityNo] = false;
        }
        else
        {
            levelUpBt.SetActive(true);

            rightBt.SetActive(false);
            leftBt.SetActive(false);
            selectBt.SetActive(false);

            isProduct[_facilityNo] = true;
        }
    }

    public async void LevelUpBt()
    {
        if (25 <= player.GetMaterialNum())
        {
            player.DownMaterialNum(25);

            isLevelUp[_facilityNo] = true;

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
            createFacility = Instantiate(createFacilitys[_selected + 6], setPosition, Quaternion.identity);
            createFacility.GetComponent<FacilityPrefab>().StartCreate();

            End();
        }
        else
        {
            MaterialShortage();
        }
    }

    void MaterialShortage()
    {
        if (materialShortageInstance != null)   Destroy(materialShortageInstance);

        materialShortageInstance.transform.SetParent(canvas.transform, false);
    }

    void End()
    {
        if (_facilityNo == 0)
        {
            PlayerPrefs.SetInt("FacilitySelect1", _selected);
        }
        else if (_facilityNo == 1)
        {
            PlayerPrefs.SetInt("FacilitySelect2", _selected);
        }
        else
        {
            PlayerPrefs.SetInt("FacilitySelect3", _selected);
        }

        Destroy(nowInstance);
        Destroy(nameInstance);
        rightBt.SetActive(false);
        leftBt.SetActive(false);
        selectBt.SetActive(false);
        exisBt.SetActive(false);
        changeBt.SetActive(false);
        levelUpBt.SetActive(false);

        if (materialShortageInstance != null) Destroy(materialShortageInstance);

        // ほかを動かしだす
        Time.timeScale = 1;
    }
}
