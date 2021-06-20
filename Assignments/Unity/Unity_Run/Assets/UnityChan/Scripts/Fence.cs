using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Fence : MonoBehaviour {

	// Use this for initialization
	void Start () {
        int idx = Random.Range(0, transform.childCount);
        transform.GetChild(idx).gameObject.SetActive(true); //활성화
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
