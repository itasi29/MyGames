using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PouseBt : MonoBehaviour
{
    Fade fade;

    [SerializeField] GameObject canvas;

    [SerializeField] GameObject battenMark;
    [SerializeField] GameObject TitleBt;

    [SerializeField] GameObject pouse;
    GameObject nowInstance;

    [SerializeField] GameObject img;
    GameObject imgInstance;

    [SerializeField] GameObject yesBt;
    [SerializeField] GameObject noBt;

    GameObject create0;
    GameObject create1;
    GameObject create2;
    GameObject levelUp;

    void Start()
    {
        fade = GameObject.Find("PlayerDirector").GetComponent<Fade>();

        create0 = GameObject.Find("Create0");
        create1 = GameObject.Find("Create1");
        create2 = GameObject.Find("Create2");
        levelUp = GameObject.Find("LevelUpBt");

    }

    public void Pouse()
    {
        Debug.Log("[Pouse]push");
        Time.timeScale = 0.0f;

        this.gameObject.SetActive(false);
        battenMark.SetActive(true);
        TitleBt.SetActive(true);
        nowInstance = Instantiate(pouse);
        nowInstance.transform.SetParent(canvas.transform, false);

        create0.SetActive(false);
        create1.SetActive(false);
        create2.SetActive(false);
        levelUp.SetActive(false);
    }

    public void ReturnGameBt()
    {
        Time.timeScale = 1.0f;

        this.gameObject.SetActive(true);
        battenMark.SetActive(false);
        TitleBt.SetActive(false);

        create0.SetActive(true);
        create1.SetActive(true);
        create2.SetActive(true);
        levelUp.SetActive(true);

        Destroy(nowInstance);
    }

    

    public void ReturnTitleBt()
    {
        Debug.Log("PUSH");

        battenMark.SetActive(false);
        TitleBt.SetActive(false);

        Destroy(nowInstance);

        imgInstance = Instantiate(img);
        imgInstance.transform.SetParent(canvas.transform, false);
        yesBt.SetActive(true);
        noBt.SetActive(true);
    }

    public void YesBt()
    {
        Time.timeScale = 1.0f;

        fade.StartFadeOut("Title");
    }
    public void NoBt()
    {
        Destroy(imgInstance);
        yesBt.SetActive(false);
        noBt.SetActive(false);

        battenMark.SetActive(true);
        TitleBt.SetActive(true);
        nowInstance = Instantiate(pouse);
        nowInstance.transform.SetParent(canvas.transform, false);
    }
}
