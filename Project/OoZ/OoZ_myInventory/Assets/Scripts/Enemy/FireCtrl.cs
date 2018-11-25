using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FireCtrl : MonoBehaviour
{

    public GameObject bullet;    //총알 프리팹
    private GameObject bulletInstance;  //생성된 총알 저장
    public Transform firePos;   //총알 발사좌표

    public float bulletSpeed = 2.0f;    //총알 속도
    public float bulletTerm = 2.0f; //총알 몇초마다 쏠지
   
    private Transform playerTr;  //Player Transform
    private Transform enemyTr;    //Enemy Transform

    public bool bulletFireCheck = false;//총알이 발사됬는지 확인
    public bool isBulletDestroyed = false;  //총알이 Destory되었는지 체크

    void OnEnable()
    {

        bulletFireCheck = false;
        StartCoroutine(FireCheck());
    }

    void Update()
    {
        //총알 위치 update
        if (bulletFireCheck == true && isBulletDestroyed == false)
        {
            bulletInstance.transform.Translate(Vector3.forward * bulletSpeed * Time.deltaTime);
        }

    }

    public void Fire(Transform playerTr_, Transform enemyTr_)
    {

        CreateBullet();
        isBulletDestroyed = false;
        bulletFireCheck = true;
        playerTr = playerTr_;
        enemyTr = enemyTr_;
    }

    void CreateBullet()
    {
        //Bullet 프리팹을 동적으로 생성
        bulletInstance = Instantiate(bullet, firePos.position, firePos.rotation);
    }

    void DestroyBullet()
    {
        Destroy(bulletInstance);
        isBulletDestroyed = true;
    }

    IEnumerator FireCheck()
    {
        while (true)
        {
            if (bulletFireCheck == true)
            {
                yield return new WaitForSeconds(bulletTerm);
                //2초 지나면 총알 없어지게
                if (isBulletDestroyed == false)
                {
                    DestroyBullet();

                }
                bulletFireCheck = false;
            }

            yield return null;
        }
    }


}
