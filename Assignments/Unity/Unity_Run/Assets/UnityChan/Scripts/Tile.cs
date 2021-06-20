using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Tile : MonoBehaviour {
    public GameObject fence;
    public Transform spawnPoint;
    public Transform nextTilePos;
    //public GameManager gameManger;
   
	// Use this for initialization
	void Start () {
        //생성하는 함수 / 게임오브젝트는 동적으로 생성 씬에 없고
        //회전단위 quqternion. 기본으로
        Instantiate(fence, spawnPoint.position, Quaternion.identity);
        GameManager.instance.nextTilePos = nextTilePos.position;
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
