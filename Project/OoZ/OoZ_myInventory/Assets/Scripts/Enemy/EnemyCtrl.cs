using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyCtrl : MonoBehaviour
{

    //적의 속도는 navAgent의 speed 수치 변경 : 지금은 7로 되어있음

    //적 상태 정보 
    public enum EnemyState { patrol, trace, attack };
    //적의 현재 상태 정보를 저장할 Enum 변수ss
    public EnemyState enemyState = EnemyState.patrol;

    private Transform tr;
    //길찾기 AI
    private UnityEngine.AI.NavMeshAgent nvAgent;

    //추적범위(Player를 발견가능한)
    public float traceDist = 20.0f;
    //총알공격범위
    public float attackDist = 7.0f;
    //총알 쐈는지 여부 확인 위한 transform
    private FireCtrl fireCtrl;
    //장애물 밟고 Stun지속시간
    public float obstacleStunTime = 5.0f;
    //Stun여부
    private bool isStun = false;
    //Stun Coroutine 
    private Coroutine stunCoroutine;

    //외부 GameObject
    private Transform playerTr;

    // Use this for initialization
    void Awake()
    {
        nvAgent = this.gameObject.GetComponent<UnityEngine.AI.NavMeshAgent>();
        playerTr = GameObject.FindWithTag("Player").GetComponent<Transform>();
        tr = this.gameObject.GetComponent<Transform>();
        fireCtrl = this.gameObject.GetComponent<FireCtrl>();
    }

    void OnEnable()
    {
        //적 행동 상태 체크
        StartCoroutine(this.CheckEnemyState());
        //적 상태에 따라 동작하는 루틴
        StartCoroutine(this.EnemyAction());
    }

    //충돌 감지
    void OnTriggerEnter(Collider col)
    {
        if (col.tag == "Obstacle")
        {
           
            stunCoroutine = StartCoroutine(GetStun());
        }
    }

    void Update()
    {
        if (isStun == false && stunCoroutine != null)
        {
            StopCoroutine(stunCoroutine);
        }
    }

    IEnumerator CheckEnemyState()
    {
        while (true)
        {
            //Debug.Log("CheckEnemyState");

            //왜하는걸까
            //yield return new WaitForSeconds(0.2f);

            if (isStun == false)
            {
                //적과 Player 사이의 거리 측정
                float dist = Vector3.Distance(playerTr.position, tr.position);

                //총알 공격 범위 이내 확인
                if (dist <= attackDist)
                {
                    enemyState = EnemyState.attack;
                }
                //추적거리 범위 이내 확인
                else if (dist <= traceDist)
                {
                    enemyState = EnemyState.trace;
                }
                //적의 상태를 patrol
                else
                {
                    enemyState = EnemyState.patrol;
                }
                //Debug.Log(dist);
                //Debug.Log(enemyState);
            }

            yield return null;
        }
    }

    IEnumerator EnemyAction()
    {
        while (true)
        {
            if (isStun == false)
            {
                //Debug.Log("EnemyAction");
                switch (enemyState)
                {
                    //patrol상태(traceDist를 벗어나있으면)
                    case EnemyState.patrol:
                        nvAgent.isStopped = false;
                        break;
                    //trace상태
                    case EnemyState.trace:
                        nvAgent.destination = playerTr.position;
                        nvAgent.isStopped = false;
                        break;
                    //attack상태(총알 공격)
                    case EnemyState.attack:
                        nvAgent.isStopped = true;
                        //총알의 쿨타임이 지났다면
                        if (fireCtrl.bulletFireCheck == false)
                        {
                            playerTr = GameObject.FindWithTag("Player").GetComponent<Transform>();
                            tr = this.gameObject.GetComponent<Transform>();
                            fireCtrl.Fire(playerTr, tr);
                        }
                        break;
                }
            }
            yield return null;
        }
    }

    IEnumerator GetStun()
    {
       
        isStun = true;
        nvAgent.isStopped = true;
        yield return new WaitForSeconds(obstacleStunTime);
        isStun = false;
        yield return null;
    }

}
