using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Start_Event : MonoBehaviour {
    public enum ANISTEP                                // 플레이어 상태 열거체
    {
        FADEIN = 1,                                  // 상태 없음
        BLANK = 2,                                   // 이동
        REFADEIN,                                       // 수리
        WAIT,                                        // 현재 상태가 몇 종류인지 나타냄
        DONE,
    };

    public GameObject img_1 = null;
    public GameObject img_2 = null;
    public GameObject img_3 = null;

    private ANISTEP step = ANISTEP.FADEIN;
    private float animation_timer = 0.0f;
    private float colorval = 0.0f;

    private Color ON = new Color(1.0f, 1.0f, 1.0f, 1.0f);
    private Color Off = new Color(1.0f, 1.0f, 1.0f, 0);
    private Color Black = new Color(0, 0, 0, 1.0f);
    private Color tcolor = new Color(1.0f, 1.0f, 1.0f, 1.0f);



    IEnumerator Fadein()
    {
        step = ANISTEP.WAIT;

        img_1.GetComponent<Image>().color = Black;
        img_2.GetComponent<Image>().color = Off;
        img_3.GetComponent<Image>().color = Off;

        yield return new WaitForSeconds(5.0f);
        
        while (colorval < 1.0f)
        {
            colorval += Time.deltaTime;

            img_1.GetComponent<Image>().color = new Color(colorval, colorval, colorval, 1.0f);

            yield return new WaitForSeconds(0.05f);
        }

        step = ANISTEP.BLANK;
    }

    IEnumerator Blank()
    {
        step = ANISTEP.WAIT;

        img_2.GetComponent<Image>().color = Black;
        

        img_2.GetComponent<Image>().color = ON;
        yield return new WaitForSeconds(0.1f);

        img_2.GetComponent<Image>().color = Black;
        yield return new WaitForSeconds(0.1f);

        img_2.GetComponent<Image>().color = ON;
        yield return new WaitForSeconds(0.1f);

        img_2.GetComponent<Image>().color = Black;
        yield return new WaitForSeconds(0.1f);

        step = ANISTEP.REFADEIN;
    }

    IEnumerator Re_Fadein()
    {
        step = ANISTEP.WAIT;

        img_1.GetComponent<Image>().color = Off;
        img_2.GetComponent<Image>().color = Off;
        img_3.GetComponent<Image>().color = Black;

        colorval = 0.0f;

        while (colorval < 1.0f)
        {
            colorval += Time.deltaTime;

            img_3.GetComponent<Image>().color = new Color(colorval, colorval, colorval, 1.0f);
            

            yield return new WaitForSeconds(0.05f);
        }

        step = ANISTEP.DONE;
    }
    // Use this for initialization
    void Start () {
        step = ANISTEP.FADEIN;

    }

    // Update is called once per frame
    void Update ()
    {

        if(animation_timer <= 0.0f && step != ANISTEP.WAIT)
        {
            switch (step)
            {
                case ANISTEP.FADEIN:
                    StartCoroutine("Fadein");
                    break;
                case ANISTEP.BLANK:
                    StartCoroutine("Blank");
                    break;
                case ANISTEP.REFADEIN:
                    StartCoroutine("Re_Fadein");
                    break;
                case ANISTEP.DONE:
                    animation_timer = 30.0f;
                    break;
            }
        }

        animation_timer -= Time.deltaTime;

        if (Input.GetKey(KeyCode.Space))
            SceneManager.LoadScene("MainScene");

    }
}
