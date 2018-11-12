using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MyColumn : MonoBehaviour {
    public float speed = -5.0f;
    public float waitSeconds = 5.0f;
    public float endOffset;

    // Use this for initialization
    void Start () {
        GetComponent<Rigidbody2D>().velocity = new Vector2(speed, 0);
	}
	
	// Update is called once per frame
	void Update () {
        Vector3 view = Camera.main.WorldToScreenPoint(transform.position);//월드 좌표를 스크린 좌표로 변형한다.
        if (view.x < endOffset)
        {
            Destroy(gameObject);    //스크린 좌표가 -2 이하일시 삭제  
        }
    }

    //void OnBecameInvisible()
    //{
    //    Destroy(gameObject);
    //}
}