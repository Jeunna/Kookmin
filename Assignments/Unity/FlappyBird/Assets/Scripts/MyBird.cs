using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyBird : MonoBehaviour {

    Rigidbody2D rigidBody;
    public Vector2 jumpForce = new Vector2(0, 200.0f);
    bool isDead = false;
    Animator animator;

	// Use this for initialization
	void Start () {
        isDead = false;
        rigidBody = GetComponent<Rigidbody2D>();
        animator = GetComponent<Animator>();
	}
	
	// Update is called once per frame
	void Update () {
        if (GameManager.Instance.isReady) return;
        else rigidBody.simulated = true;

        //추상화 되어있어서 좋. 모바일은 알아서 클릭으로.
        if (Input.GetMouseButtonDown(0))
            Jump();
	}

    void Jump()
    {
        //객체가 계속 생성되니까 가비지 컬렉터가 돌아감. c#이나 자바는 직접제거가 없으니까 가비지에 점점 쌓이겠지
        //1. 가비지 컬렉터한테 먹이를 주지마라. new를 쓰지마라. 업데이트 안에 뉴가 있으면 안돼
        //rigidBody.AddForce(new Vector2(0, jumpForce));

        rigidBody.velocity = Vector2.zero;
        rigidBody.AddForce(jumpForce);

        animator.SetTrigger("Jump");
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (!isDead)
        {
            isDead = true;
            animator.SetTrigger("Die");
            GameManager.Instance.GameOver();
        }
    }

    // 트리거로 만들면 이걸로 호출됨
    private void OnTriggerEnter2D(Collider2D collision)
    {
        GameManager.Instance.AddScore();
    }
}
