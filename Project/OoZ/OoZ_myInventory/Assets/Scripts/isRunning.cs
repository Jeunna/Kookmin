using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Spine.Unity;

public class isRunning : StateMachineBehaviour {
    public string animationName;
    public float speed;

    public void OnStateEnter(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    {
        SkeletonAnimation anime = animator.GetComponent<SkeletonAnimation>();
        anime.state.SetAnimation(0, animationName, true).timeScale = speed;
    }
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
