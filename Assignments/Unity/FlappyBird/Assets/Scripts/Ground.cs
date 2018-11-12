using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ground : MonoBehaviour {
    public float endOffset;
    public float scrollSpeed = 1;
    //스피드곱하기델타타임
    Rigidbody2D rb2D;

    // Use this for initialization
    void Start () {
        rb2D = GetComponent<Rigidbody2D>();
        //키네마틱이라 속도가 감속되지않음 계속 똑같은 스피드로 움직임
        rb2D.velocity = new Vector2(scrollSpeed, 0);
    }
	
	// Update is called once per frame
	void Update () {

        // x의 위치가 offset보다 작아지면 위치를 앞으로 옮긴다
        if (transform.position.x <= endOffset)
        {
            float width = GetComponent<BoxCollider2D>().size.x;
            Vector3 pos = transform.position; // x값을 가져올 수 있지만 바꾸려면 다같이 가져와야함 프로포트랬나 transform.position.x = 1.0f하면 알림뜸
            pos.x += width * 2 - 0.01f; // 두칸앞으로 가야함, - 1픽셀 차이나는거
            transform.position = pos;
        }
	}


    //private void OnBecameInvisible()
    //{
    //    GameObject prevGround = GameManager.Instance.currGround;
    //    Vector3 pos = prevGround.transform.position;
    //    float size = prevGround.GetComponent<BoxCollider2D>().size.x;
    //    //pos - size 하면 내 위치값
    //    pos.x += size;
    //    transform.position = pos;
    //}

    //private void OnBecameVisible()
    //{
    //    //this는 클래스의 인스턴스
    //    //소문자 게임오브젝트는 자기자신꺼
    //    GameManager.Instance.currGround = gameObject;
    //}
}
