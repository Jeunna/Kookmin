using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI; //text

public class GameManager : MonoBehaviour {
    public Vector3 nextTilePos;
    public GameObject tilePrefab;
    public GameObject readyImage;
    public Text scoreText;
    public Text timerText;

    //싱거톤.. 하나 만들어놓고 아무데서나 쓸수잇게 해주는거
    public static GameManager instance;

    public bool isReady = true;
    int score = 0;
    float timer = 0;

    //생성자 함수, start보다 빨리
    private void Awake()
    {
        instance = this;
    }

    // Use this for initialization
    void Start () {
        
	}

    // Update is called once per frame
    void LateUpdate () {
		if(isReady)
        {
            if (Input.GetMouseButtonDown(0))
            {
                isReady = false;
                //없애는게 아니고 껐다켰다. 생성이 늦어요
                readyImage.SetActive(false);
            }
        }

        if (!isReady)
        {
            timer += Time.deltaTime;
            timerText.text = "Time : " + timer.ToString("0.0");
        }
	}

    //생성될 다음 위치는 타일이 알고 있으니까 타일이 위치 알려주면됨
    //타일의 중간쯤 지나갈때 타일을 생성하고 싶다. -> 어떤 지점을 지나갈때 생성하먄 됨. (트리거)
    //다른 곳에서 호출할라면 public
    public void SpawnTile()
    {
        Instantiate(tilePrefab, nextTilePos, Quaternion.identity);
    }

    public void AddScore()
    {
        ++score;
        scoreText.text = "Score : " + score.ToString();
    }
}
