using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PouseBt : MonoBehaviour
{
    [SerializeField] GameObject canvas;

    [SerializeField] GameObject battenMark;
    [SerializeField] GameObject TitleBt;

    [SerializeField] GameObject pouse;
    GameObject nowInstance;

    [SerializeField] GameObject img;
    GameObject imgInstance;

    [SerializeField] GameObject yesBt;
    [SerializeField] GameObject noBt;


    public void Pouse()
    {
        Debug.Log("[Pouse]push");
        Time.timeScale = 0.0f;

        this.gameObject.SetActive(false);
        battenMark.SetActive(true);
        TitleBt.SetActive(true);
        nowInstance = Instantiate(pouse);
        nowInstance.transform.SetParent(canvas.transform, false);
    }

    public void ReturnGameBt()
    {
        Time.timeScale = 1.0f;

        this.gameObject.SetActive(true);
        battenMark.SetActive(false);
        TitleBt.SetActive(false);

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

        SceneManager.LoadScene("Title");
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
