using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement; //씬 관리

public class Player : MonoBehaviour {
    public float speed = 1.0f;
    public float jumpForce = 1000.0f;
    bool isJump = false; //몇단 조건을 막기위해
    //public GameManager gameManger;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        if (GameManager.instance.isReady) return;

        //jump
        //컴퓨터는 마우스 클릭, 모바일은 터치로 자동 인식해줌
        if (Input.GetMouseButtonDown(0) && !isJump)
        {
            //transform말고는 컴포넌트 가져오는 방법은 getcomponetn임
            GetComponent<Rigidbody>().AddForce(0, jumpForce, 0);
            isJump = true;
            //컴포넌트 단위로 생각해야함. Jump는 애니메이터에서 설정한 거니까
            GetComponent<Animator>().SetBool("Jump", true);
        }

        //컴퓨터 성능에 따라서 결과가 다르면 안되니까 결과는 같되 움직임이 몇 번으로 쪼개져서 보이냐의 차이로 가야한다
        //그래서 프레임기준이 아니고 시간의 기준. 그래서 엔진은 한 프레임에 걸리는 시간을 줌. == 델타타임
        //transform.Translate(0, 0, speed);

        //run
        transform.Translate(0, 0, speed * Time.deltaTime); //초속 1유닛을감 (대충 1미터로 생각하셈)
	}

    //충돌됐을때 자동적으로 콜백되는 콜백함수
    private void OnCollisionEnter(Collision collision)
    {
        //collision은 충돌한 위치, 힘을 갖고있음. 그래서 게임오브젝트를 불러와서..tag...
        if(collision.gameObject.CompareTag("Floor"))
        {
            isJump = false;
            GetComponent<Animator>().SetBool("Jump", false);
        }

        if(collision.gameObject.CompareTag("Block"))
        {
            //Restart Game
            SceneManager.LoadScene(0); //로딩창, 씬이 하나밖에 없으니까 0번
            //0번 창을 로딩하라는거지 restart하라는게 아님. (샘플씬이 0번씬임) ㄹㅇ컴포넌트 다 빼고 창만 로딩됨.
            //로딩은 비동기로딩으로. 로드어싱크. 창은 로딩창 띄우고 밑에선 로딩돌아가고 있어야죠
            //씬체크 중요! -> 파일에 빌드 세팅에서 체크가능
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        GameManager.instance.SpawnTile();
        GameManager.instance.AddScore();
    }
}
