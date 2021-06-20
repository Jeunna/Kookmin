using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HelloUnity : MonoBehaviour {
    //public으로 만들면 인스펙터에 표시됨
    public float rotSpeed = 1.0f;

	// Use this for initialization
	void Start () {
        Debug.Log("Hello Unity");
	}
	
	// Update is called once per frame
	void Update () {
        //캐릭터 회전 (model 값 변경이 아니라 transform 컴포턴트 값 변경
        transform.Rotate(0, rotSpeed, 0); //매프레임마다 1도씩 회전함
        
	}
}
