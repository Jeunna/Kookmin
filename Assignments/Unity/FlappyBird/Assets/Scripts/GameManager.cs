using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class GameManager : Singleton<GameManager> {
    public Vector3 gameOverUIMoveFrom;
    public float gameOverUITime = 1.0f;
    public bool isGameOver = false;
    public GameObject gameOverUI;
    float score = 0;
    public Text scoreText;
    public float spawnColumnX;
    public GameObject column; //프리팹을 받을거임
    public float colMinY;
    public float colMaxY; //나중에 고치기 편할라고
    public float spawnTime = 1;
    public float takeSecond = 2;

    public GameObject currGround;
    //확인할땐 비져블로 해도되고 위치계산을 해서 해줘도됨

    public bool isReady = true;
    public GameObject readyText;
    public bool startColumn = false;

    // Use this for initialization
    void Start () {    
        isReady = true;
        readyText.SetActive(true);
        score = 0; // 싱글톤이니까 초기화해줌
        isGameOver = false;
        gameOverUI.SetActive(false);
    }
	
	// Update is called once per frame
	void Update () {
        if (isReady)
        {
            if (Input.GetMouseButtonDown(0))
            {
                isReady = false;
                readyText.SetActive(false);
                StartCoroutine(SpawnColumn()); // 게임이 오버될때까지 계속 생성됨
            }
            return;
        }

        else if (isGameOver && Input.GetMouseButtonDown(0))
        {
            SceneManager.LoadScene(0);
        }
            
	}

    public void GameOver()
    {
        isGameOver = true;

        GameObject[] groundArray = GameObject.FindGameObjectsWithTag("Ground");
        for (int i = 0; i < groundArray.Length; ++i)
            groundArray[i].GetComponent<Rigidbody2D>().velocity = new Vector2(0, 0);
            

        GameObject[] columnArray = GameObject.FindGameObjectsWithTag("Column");
        for (int i = 0; i < columnArray.Length; ++i)
            columnArray[i].GetComponent<Rigidbody2D>().velocity = new Vector2(0, 0);

        //GameObject.FindWithTag("Ground").GetComponent<Rigidbody2D>().velocity = new Vector2(0, 0);
        //GameObject.FindWithTag("Column").GetComponent<Rigidbody2D>().velocity = new Vector2(0, 0);

        gameOverUI.SetActive(true);
        //여기로 오라니까 시작점을 주면됨
        iTween.MoveFrom(gameOverUI, gameOverUIMoveFrom, gameOverUITime);
    }

    public void AddScore()
    {
        ++score;
        scoreText.text = "SCORE : " + score;
    }

    // 코루틴 전용 리턴값
    IEnumerator SpawnColumn()
    {
        // 무한루프라 update에서 생성하면 계속 코루틴이 생성돼서 무한루프임
        while (!isGameOver)
        {
            Vector3 pos = new Vector3();
            pos.x = spawnColumnX;
            pos.y = Random.Range(colMinY, colMaxY);
            Instantiate(column, pos, Quaternion.identity);

            // 시간이나 프레임 반환. 그 시간이후에 다시 실행시켜줘 or 1프레임후에 다시 실행시켜줘
            // 양보하는 리턴 양보?
            yield return new WaitForSeconds(spawnTime);
        }
    }
}
