using System.Collections;
using System.Collections.Generic;
using Spine.Unity;
using UnityEngine;

public class Player : MonoBehaviour
{

    // ----------------------- Player 기본 변수 부분(유영준) ------------------------------
    public bool isStun = false;
    public static float STUN_TIME_BULLET = 1.5f;
    public float stun_timer_bullet = 0.0f;

    // ----------------------- 외부 변수 가져온거(유영준) ------------------------------
    public FireCtrl fireCtrl;

    // ----------------------- 인벤토리 ------------------------------

    public Inventory inventory;
    public HUD hud;
    private InventoryItemBase mCurrentItem = null;
    private InteractableItemBase mInteractItem = null;

    // ----------------------- 키 입력 관련 부분 ------------------------------
    public float MOVE_SPEED = 5.0f;          // 이동 속도

    private int clickLayer = 10;             // 이동 가능한 layer를 클릭했는지?
    Vector3 hitPosition;
    public CharacterController cc;


    private struct Key                              // 키 조작 정보
    {
        public bool up;
        public bool down;
        public bool right;
        public bool left;
        public bool mouse_move;

        public bool pick;                           // 줍는다/버린다 Z키
        public bool action;                         // 먹는다/수리한다 X키
        public bool lit_Fire;                        // 불피우기 Spacebar
        public bool pick_item;

        public bool skill_change_left;              // 스킬교체 A키
        public bool skill_ON;                       // 스킬사용 S키
        public bool skill_OFF;                      // 스킬사용 S키 해제
        public bool skill_change_right;             // 스킬교체 D키
    }

    private Key key;                                // 키 조작 보관 변수 선언

    public enum STEP                                // 플레이어 상태 열거체
    {
        NONE = -1,                                  // 상태 없음
        MOVE = 0,                                   // 이동
        MOUSE_MOVE,
        EATING,                                     // 식사
        STUN,                                       // 스턴
        NUM,                                        // 현재 상태가 몇 종류인지 나타냄
    };

    public STEP step = STEP.NONE;                   // 현재 상태
    public STEP next_step = STEP.NONE;              // 다음 상태
    public float step_timer = 0.0f;                 // 타이머

    private void Get_Input()                        // key값 갱신 method
    {
        this.key.up = false;
        this.key.down = false;
        this.key.right = false;
        this.key.left = false;
        this.key.mouse_move = false;

        this.key.pick = false;
        this.key.action = false;
        this.key.lit_Fire = false;
        this.key.pick_item = false;

        this.key.skill_change_left = false;
        this.key.skill_ON = false;
        this.key.skill_OFF = false;
        this.key.skill_change_right = false;


        // |= 연산은 A |= B; 라면 A = A|B; 의 역할을 합니다.
        // A, B 둘 다 false일 때만 false를 반환합니다.
        // up키가 눌리면 true 대입
        this.key.up |= Input.GetKey(KeyCode.UpArrow);
        this.key.up |= Input.GetKey(KeyCode.Keypad8);

        // down키가 눌리면 true 대입
        this.key.down |= Input.GetKey(KeyCode.DownArrow);
        this.key.down |= Input.GetKey(KeyCode.Keypad2);

        // right키가 눌리면 true 대입
        this.key.right |= Input.GetKey(KeyCode.RightArrow);
        this.key.right |= Input.GetKey(KeyCode.Keypad6);

        // left키가 눌리면 true 대입
        this.key.left |= Input.GetKey(KeyCode.LeftArrow);
        this.key.left |= Input.GetKey(KeyCode.Keypad4);

        // 마우스로 이동 조작, 버튼 up 때 true 대입
        this.key.mouse_move |= Input.GetMouseButtonUp(1);


        // 아래는 연속입력을 방지하기 위해 GetKeyDown을 사용합니다.
        // Z키가 눌리면 true 대입 (줍는다/버린다)
        this.key.pick |= Input.GetKeyDown(KeyCode.Z);
        // X키가 눌리면 true 대입 (먹는다/수리한다)
        this.key.action |= Input.GetKeyDown(KeyCode.X);
        // Spacebar키가 눌리면 true 대입, 횃불을 지핀다.
        this.key.lit_Fire |= Input.GetKeyDown(KeyCode.Space);
        // item 줍기 조작
        this.key.pick_item |= Input.GetMouseButtonDown(0);

        // S키가 눌리면 true 대입 (스킬사용), A,D로 스킬 교체
        this.key.skill_change_left |= Input.GetKeyDown(KeyCode.LeftShift);
        this.key.skill_ON |= Input.GetKeyDown(KeyCode.LeftControl);
        this.key.skill_OFF |= Input.GetKeyUp(KeyCode.LeftControl);
        this.key.skill_change_right |= Input.GetKeyDown(KeyCode.RightShift);



    }

    public GameObject Now = null;
    public GameObject Front = null;
    public GameObject Left = null;
    public GameObject Right = null;
    public GameObject Back = null;
    
    private void Set_Spine_Direction(bool dir1, bool dir2, bool dir3, bool dir4)
    {
        Front.SetActive(dir1);
        Left.SetActive(dir2);
        Right.SetActive(dir3);
        Back.SetActive(dir4);

        if (dir1)
        {
            Now = Front;
        }
        if (dir2)
        {
            Now = Left;
        }
        if (dir3)
        {
            Now = Right;
        }
        if (dir4)
        {
            Now = Back;
        }

        //Now = Right.GetComponent<SkeletonAnimation>();
        //Set_Spine_Animation("run", true, 1.0f);
    }
    

    private void Move_Control()       // 키보드 이동
    {
        Vector3 move_vector = Vector3.zero;         // 플레이어 이동용 Vector
        Vector3 position = this.transform.position; // 현재 위치를 보관하는 Vector


        do
        {
            if (this.key.up)
            {
                move_vector += Vector3.forward;         // move_vector(이동용 Vector)를 위쪽으로 변경
                break;
            }

            if (this.key.down)
            {
                move_vector += Vector3.back;
            }
        } while (false);
        do
        {
            if (this.key.right)
            {
                move_vector += Vector3.right;
                break;
            }

            if (this.key.left)
            {
                move_vector += Vector3.left;
            }
        } while (false);


        move_vector.Normalize();                    // Vector 길이를 1로 정규화
        move_vector *= MOVE_SPEED * Time.deltaTime; // 거리 = 속도 x 시간
        position += move_vector;                    // 플레이어 위치를 이동                       
        position.y = 0.0f;                          // 플레이어 높이를 0으로 한다

        position.y = this.transform.position.y;     // 새로 구한 위치의 높이를 현재 높이로
        this.transform.position = position;         // 새로 구한 위치를 현재 위치로



        do
        {
            if (move_vector.x > 0.0f) // 오른
            {
                Set_Spine_Direction(false, false, true, false);
                break;
            }

            if (move_vector.x < 0.0f) // 왼
            {
                Set_Spine_Direction(false, true, false, false);
                break;
            }

            if (move_vector.z > 0.0f) // 위
            {
                Set_Spine_Direction(false, false, false, true);
                break;
            }

            if (move_vector.z <= 0.0f) //아래
            {
                Set_Spine_Direction(true, false, false, false);
            }
        } while (false); // 스파인 애니메이션 방향별 출력


        // 이동을 했다면?
        if (move_vector.magnitude > 0.01f)
        {// 캐릭터의 방향을 바꾼다.
            Quaternion q = Quaternion.LookRotation(move_vector, Vector3.up);
            this.transform.rotation = q;            // Lerp 빼버렸다.
        }

    }

    private Vector3 targetPos = Vector3.zero;
    private Vector3 framePos = Vector3.zero;
    private void Mouse_Move_Control() // 마우스 이동
    {

        if (this.key.mouse_move)
        {

            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

            RaycastHit hitInfo;

            if (Physics.Raycast(ray, out hitInfo, 100f))
            {
                int l = hitInfo.transform.gameObject.layer;

                if (l == clickLayer)
                {
                    hitPosition = hitInfo.point;
                    hitPosition.y = this.transform.position.y;
                }

            }
        }

        Vector3 move_vector = Vector3.zero;         // 플레이어 이동용 Vector
        Vector3 position = this.transform.position; // 현재 위치를 보관하는 Vector

        move_vector = hitPosition - transform.position;

        move_vector.Normalize();                    // Vector 길이를 1로 정규화
        move_vector *= MOVE_SPEED * Time.deltaTime; // 거리 = 속도 x 시간
        position += move_vector;                    // 플레이어 위치를 이동                       
        position.y = 0.0f;                          // 플레이어 높이를 0으로 한다

        position.y = this.transform.position.y;     // 새로 구한 위치의 높이를 현재 높이로
        this.transform.position = position;         // 새로 구한 위치를 현재 위치로

        do
        {
            if (move_vector.x > 0.0f) // 오른
            {
                Set_Spine_Direction(false, false, true, false);
                break;
            }

            if (move_vector.x < 0.0f) // 왼
            {
                Set_Spine_Direction(false, true, false, false);
                break;
            }

            if (move_vector.z > 0.0f) // 위
            {
                Set_Spine_Direction(false, false, false, true);
                break;
            }

            if (move_vector.z <= 0.0f) //아래
            {
                Set_Spine_Direction(true, false, false, false);
            }
        } while (false); // 스파인 애니메이션 방향별 출력

        Quaternion q = Quaternion.LookRotation(targetPos, Vector3.up);
        this.transform.rotation = q;            // Lerp 빼버렸다.

    }


    // ----------------------- 수집 관련 부분 ------------------------------

    private GameObject closest_item = null;         // 플레이어의 정면에 있는 게임 오브젝트
    private GameObject carried_item = null;         // 플레이어가 들어 올린 게임 오브젝트
    private ItemRoot item_root = null;              // ItemRoot 스크립트를 가져온다
    public GUIStyle guistyle;                       // Font 스타일

    private void Pick_Or_Drop_Control()             // 물건을 줍거나 떨어뜨리기 위한 method 입니다.
    {
        do
        {
            if (!this.key.pick)                                                             // '줍기/버리기' Z키가 눌리지 않았으면 종료
            {
                break;
            }

            // 주목중인 아이템을 인벤토리로
            InteractWithItem();

            // 주목 중인 아이템을 삭제
            this.closest_item = null;
            mInteractItem = null;

        } while (false);
    }
    private bool Is_Other_In_View(GameObject other) // 접촉한 물건이 자신의 정면에 있는지 판정하는 method 입니다.
    {
        bool ret = false;
        do
        {
            Vector3 heading = this.transform.TransformDirection(Vector3.forward);
            // 자신이 현재 향하고 있는 방향
            Vector3 to_other = other.transform.position - this.transform.position;
            // 자신 쪽에서 본 아이템의 방향

            heading.y = 0.0f;                               // Player 의 Item 의 높이를 같은 선상으로 한 후
            heading.Normalize();                            // Player 의 방향벡터의 길이를 1로 정규화 시킵니다.
            to_other.y = 0.0f;                              // Item 과 Player 의 높이를 같은 선상으로 한 후
            to_other.Normalize();                           // Item 의 방향벡터의 길이를 1로 정규화 시킵니다.

            float dp = Vector3.Dot(heading, to_other);      // 양쪽 벡터의 내적을 얻습니다.
            if (dp < Mathf.Cos(45.0f))                      // 내적이 45도 cosine 값 미만이라면? -> 루프를 빠져나갑니다.
            {
                break;
            }
            ret = true;                                     // 내적이 45도 cosine 값 이상이면 정면에 있다고 판정합니다.
        } while (false);
        return ret;

        /*
         * 벡터의 내적이 기억이 잘 안나서 정리합니다.
         *  vector A 와 vector B 의 내적은 A·B 로 나타내며,
         *  A·B = |A| * |B| * cosθ 입니다.
         *  
         * 여기서 내적이 사용된 이유는 Player의 인지 범위(45도) 내에 Item이 있는지 판별하기 위함입니다.
         * 그렇다면 각도를 비교해야 하지만 그런 프로그래밍적 명령은 없나 봅니다.
         * 
         * 각도를 비교할 수 없기 때문에 내적을 사용해 비교하기 위해 다음과 같은 과정을 거쳤습니다.
         *  1. Mathf.Cos(45.0f)는 각 벡터의 길이가 1이고 각도가 45도인 두 벡터의 내적을 의미합니다.
         *  2. 때문에 Player 의 방향벡터, Item 의 방향벡터의 길이를 1로 정규화 합니다.
         *  3. 두 방향벡터의 내적을 구합니다 = dp
         *  4. dp와 Mathf.Cos(45.0f)를 비교합니다.
         * 
         * 내적이 여기서 나오네
         */
    }
    private bool Is_Carried_Food()                       // 들고 있는 물건이 먹을것인지 판별하는 method입니다.
    {
        if (this.carried_item != null)
        {
            Item.TYPE carried_item_type = this.item_root.GetItemType(this.carried_item);

            switch (carried_item_type)              // 가지고 있는 아이템을 판별합니다.
            {
                case Item.TYPE.APPLE:               // 사과라면
                case Item.TYPE.PLANT:               // 혹은 식물이라면
                    return true;
            }
        }
        return false;
    }

    // 주목한다는 것은 Player가 떨어져 있는 Item을 보고 있음을 의미합니다.
    void OnTriggerStay(Collider other)              // Trigger에 걸린 GameObject를 주목합니다.
    {
        GameObject other_go = other.gameObject;

        // Trigger의 GameObject Layer 설정이 Item이면~
        if (other_go.layer == LayerMask.NameToLayer("Item"))
        {
            if (this.closest_item == null)                  // 아무것도 주목하고 있지 않다면?
            {
                if (this.Is_Other_In_View(other_go))        // 정면에 아이템이 있으면?
                {
                    this.closest_item = other_go;           // 주목한다.
                }
            }
            else if (this.closest_item == other_go)         // 지금 뭔가 주목하고 있다면?
            {
                if (!this.Is_Other_In_View(other_go))       // 정면에 아이템이 없으면?
                {
                    this.closest_item = null;               // 주목을 그만둔다.
                }
            }
        }
    }
    void OnTriggerExit(Collider other)              // 주목을 그만둡니다.
    {
        if (this.closest_item == other.gameObject)
        {
            this.closest_item = null;
        }
    }




    // ----------------------- 스킬 사용 부분 ------------------------------
    private GameObject carried_skill = null;
    private SkillRoot skill_root = null;
    private int Skill_Pointer = 0;

    private void Skill_Use_Control()             // 스킬 사용하는 method 입니다.
    {
        switch (this.carried_skill.tag)
        {
            case "Owl":
                this.skill_root.UseOwl();
                break;
            case "Alpaca":
                this.skill_root.UseAlpaca();
                break;
            case "Turtle":
                this.skill_root.UseTurtle();
                break;
        }
    }

    private void Skill_Reset_Control()             // 스킬을 취소하는 method 입니다.
    {
        switch (this.carried_skill.tag)
        {
            case "Owl":
                this.skill_root.resetOwl();
                break;
            case "Alpaca":
                this.skill_root.resetAlpaca();
                break;
            case "Turtle":
                this.skill_root.resetTurtle();
                break;
        }
    }

    private void Skill_Change_Control()
    {
        if (this.key.skill_change_left)
        {
            this.Skill_Pointer--;
            if (this.Skill_Pointer == -1)
                this.Skill_Pointer = this.skill_root.Skill_list.Count - 1;
        }
        else
        {
            this.Skill_Pointer++;
            if (this.Skill_Pointer == this.skill_root.Skill_list.Count)
                this.Skill_Pointer = 0;
        }

        this.carried_skill = this.skill_root.Skill_list[Skill_Pointer];
    }

    // ------------------ 화면 UI -------------------
    void OnGUI()
    {
        // x 는 값을 더하면 오른쪽으로, 빼면 왼쪽으로
        float x = 20.0f;
        // y 는 값을 더하면 아래로, 빼면 위로
        float y = Screen.height - 40.0f;

        if (this.carried_item != null)              // 들고 있는 Item이 있다면
        {
            
            //hud.OpenMessagePanel("Press Z to drop / Press X to eat");
            //GUI.Label(new Rect(x, y, 200.0f, 20.0f), "Z : 버린다", guistyle);
            //GUI.Label(new Rect(x + 100.0f, y, 200.0f, 20.0f), "X : 먹는다", guistyle);
        }
        else
        {
            if (this.closest_item != null)           // 들고있지 않은데 주목하고 있다면
            {
                //hud.OpenMessagePanel("Press Z to pick up");
                InteractableItemBase item = this.closest_item.GetComponent<InteractableItemBase>();

                if (item.CanInteract(this.closest_item))
                {
                    mInteractItem = item;
                    
                    hud.OpenMessagePanel(mInteractItem);
                }
                hud.OpenElementalBubble("들어보자");
                //GUI.Label(new Rect(x, y, 200.0f, 20.0f), "Z : 줍는다", guistyle);
            }
            else { hud.CloseMessagePanel(); hud.CloseElementalBubble(); }
        }

        switch (this.step)
        {
            case STEP.EATING:
                GUI.Label(new Rect(x, y, 200.0f, 20.0f), "암냠냠", guistyle);
                break;
        }

        GUI.Label(new Rect(x, y - 20.0f, 200.0f, 20.0f), "◀", guistyle);
        GUI.Label(new Rect(x + 100.0f, y - 20.0f, 200.0f, 20.0f), "▶", guistyle);
        switch (this.carried_skill.tag)
        {
            case "Owl":
                GUI.Label(new Rect(x + 30.0f, y - 20.0f, 200.0f, 20.0f), "부엉이", guistyle);
                break;
            case "Alpaca":
                GUI.Label(new Rect(x + 30.0f, y - 20.0f, 200.0f, 20.0f), "알파카", guistyle);
                break;
            case "Turtle":
                GUI.Label(new Rect(x + 30.0f, y - 20.0f, 200.0f, 20.0f), "거북이", guistyle);
                break;
        }
    }



    // ------------------- 게임 플레이 --------------------
    public GameObject basic_light = null;
    public GameObject torch_light = null;
    public float go_dark = 0.1f;
    public float go_bright = 0.1f;

    // basic_light range = 8 ~ 12 
    // torch_light range = 8 ~ 12

    public float light_timer = 30.0f;

    bool is_carried_Stick() // 인벤토리 내에 나뭇가지 소지시 true 반환, 나중에 고쳐야함 --------------------
    {
        return true;
    }
    IEnumerator goDarkness()                            // 횃불 꺼짐.
    {
        Light tBasic = basic_light.GetComponent<Light>();
        Light tTorch = torch_light.GetComponent<Light>();
        float range_instance = tBasic.range + tTorch.range - 2.0f;

        while (tBasic.range + tTorch.range > range_instance)
        {
            if (tBasic.range + tTorch.range < 16.0f)     // 밝기 최소시 탈출
                break;

            tBasic.range -= go_dark;
            tTorch.range -= go_dark;

            yield return new WaitForSeconds(0.01f);
        }

    }
    IEnumerator litFire()                               // 횃불 지피기. 코루틴
    {
        Light tBasic = basic_light.GetComponent<Light>();
        Light tTorch = torch_light.GetComponent<Light>();
        float range_instance = tBasic.range + tTorch.range + 2.0f;

        while (tBasic.range + tTorch.range < range_instance)
        {
            if (tBasic.range + tTorch.range > 24.0f)    // 밝기 최대시 탈출
                break;

            if (is_carried_Stick())                                // Stick을 가지고 있다면
            {
                tBasic.range += go_bright;
                tTorch.range += go_bright;

            }
            yield return new WaitForSeconds(0.01f);
        }

    }

    // ---------- 충돌처리 관련 부분입니다.
    public Transform warpTarget = null;
    void OnTriggerEnter(Collider col)
    {
        if (col.gameObject.tag == "Warp")
        {
            warpTarget = col.transform.GetChild(0).transform;
            this.transform.position = warpTarget.position;
        }

        //스턴상태가 아닐때 총알에 맞았을 경우
        if (col.tag == "Bullet")
        {
            Destroy(col.gameObject);
            fireCtrl.isBulletDestroyed = true;
            //총알 맞았으면 isStun true로
            isStun = true;
        }
    }



    // ------------------- 인벤토리 관련 --------------------

    private void Inventory_ItemRemoved(object sender, InventoryEventArgs e)
    {
        InventoryItemBase item = e.Item;

        GameObject goItem = (item as MonoBehaviour).gameObject;
        //Debug.Log("player_removed : " + goItem);
        goItem.SetActive(true);
        goItem.transform.parent = null;
    }

    private void SetItemActive(InventoryItemBase item, bool active)
    {
        GameObject currentItem = (item as MonoBehaviour).gameObject;
        currentItem.SetActive(active);
        currentItem.transform.parent = active ? currentItem.transform : null;
        currentItem.transform.localPosition = Vector3.right;
    }

    //아이템 손에 들기
    private void Inventory_ItemUsed(object sender, InventoryEventArgs e)
    {
        //if(e.Item.ItemType != Item.CanUsed) { } 들수있는거만 정하고 싶으면

        if(mCurrentItem != null)
        {
            SetItemActive(mCurrentItem, false);
        }

        InventoryItemBase item = e.Item;

        // Use item (put it to hand of the player)
        //SetItemActive(item, true);

        if (!item.Used)
        {
            item.Used = true;
            SetItemActive(item, true);
        }
        else
        {
            item.Used = false;
            SetItemActive(item, false);
        }

        mCurrentItem = e.Item;

        //if (!item.Used)
        //{
        //    item.Used = true;
        //    mCurrentItem.SetActive(true);

        //    mCurrentItem.transform.parent = this.transform;
        //    mCurrentItem.transform.localPosition = Vector3.right;
        //}
        //else
        //{
        //    item.Used = false;
        //    mCurrentItem.SetActive(false);
        //}

    }

    //private int Attack_1_Hash = Animator.StringToHash("Base Layer.Attack_1");

    //public bool IsAttacking
    //{
    //    get
    //    {
    //        AnimatorStateInfo stateInfo = _animator.GetCurrentAnimatorStateInfo(0);
    //        if (stateInfo.fullPathHash == Attack_1_Hash)
    //        {
    //            return true;
    //        }
    //        return false;
    //    }
    //}

    public void DropCurrentItem()
    {
        //_animator.SetTrigger("tr_drop");

        GameObject goItem = (mCurrentItem as MonoBehaviour).gameObject;

        inventory.RemoveItem(mCurrentItem);

        // Throw animation
        Rigidbody rbItem = goItem.AddComponent<Rigidbody>();
        if (rbItem != null)
        {
            rbItem.AddForce(transform.forward * 2.0f, ForceMode.Impulse);

            Invoke("DoDropItem", 0.25f);
        }

    }

    public void DoDropItem()
    {

        // Remove Rigidbody
        Destroy((mCurrentItem as MonoBehaviour).GetComponent<Rigidbody>());

        mCurrentItem = null;
    }

    public bool IsDead = false;
    void FixedUpdate()
    {
        if (!IsDead)
        {
            // Drop item
            if (mCurrentItem != null && Input.GetKeyDown(KeyCode.R))
            {
                DropCurrentItem();
            }
        }
    }


    void Start()
    {
        // 이동, 수리, 식사 등 상태에 관한 초기화입니다
        this.step = STEP.NONE;                      // 현재 상태 초기화
        this.next_step = STEP.NONE;                 // 다음 상태 초기화

        // 수집 행동을 위해 ItemRoot 스크립트를 가져옵니다.
        this.item_root = GameObject.Find("GameRoot").GetComponent<ItemRoot>();
        this.guistyle.fontSize = 16;

        this.skill_root = GameObject.Find("GameRoot").GetComponent<SkillRoot>();
        this.carried_skill = skill_root.Skill_list[0];    // 스킬 설정

        inventory.ItemUsed += Inventory_ItemUsed;
        inventory.ItemRemoved += Inventory_ItemRemoved;

        //fireCtrl가져온거
        fireCtrl = GameObject.FindGameObjectWithTag("Enemy").GetComponent<FireCtrl>();
    }



    void Update()
    {

        this.Get_Input();                           // 키 입력정보 취득
        this.step_timer += Time.deltaTime;
        float eat_time = 2.0f;

        // 상태를 변화시키는 처리
        // 시작시 step = NONE, next_step = NONE
        switch (this.step)                      // 현재 상태가...
        {
            case STEP.NONE:
                if (isStun == true)
                {
                    this.next_step = STEP.STUN;
                    break;
                }
                if (this.key.up || this.key.down || this.key.left || this.key.right)
                {
                    this.next_step = STEP.MOVE;
                    break;
                }
                if (this.key.mouse_move)
                {
                    this.next_step = STEP.MOUSE_MOVE;
                    break;
                }
                if (!this.key.action)       // 액션(X)키가 눌려 있지 않다 -> 루프 탈출
                {
                    break;  // 탈출
                }

                if (Is_Carried_Food())
                {
                    this.next_step = STEP.EATING;   // 상태를 '식사'로 바꿉니다.
                    break; // 탈출
                }
                break;
            case STEP.MOVE:                     // 키보드 이동중일 때
                if (isStun == true)
                {
                    this.next_step = STEP.STUN;
                    break;
                }

                if (!this.key.action)       // 액션(X)키가 눌려 있지 않다 -> 루프 탈출
                {
                    if (!this.key.up && !this.key.down && !this.key.left && !this.key.right)
                    {
                        this.next_step = STEP.NONE;
                    }
                    break;  // 탈출
                }

                if (Is_Carried_Food())
                {
                    this.next_step = STEP.EATING;   // 상태를 '식사'로 바꿉니다.
                    break; // 탈출
                }
                break;
            case STEP.MOUSE_MOVE:                     // 마우스 이동중일 때
                if (isStun == true)
                {
                    this.next_step = STEP.STUN;
                    break;
                }

                if (Vector3.Distance(hitPosition, this.transform.position) <= 0.5f) // 마우스 클릭 위치 도착
                {
                    this.next_step = STEP.NONE;
                    break;  // 탈출
                }

                if (Is_Carried_Food())
                {
                    this.next_step = STEP.EATING;   // 상태를 '식사'로 바꿉니다.
                    break; // 탈출
                }

                break;
            case STEP.EATING:
                if (this.step_timer > eat_time)         // 아래서 상태 변할때 step_timer를 0으로 만들어 2초간 돌게된다.
                {
                    this.next_step = STEP.NONE;
                }
                break;

            case STEP.STUN: //스턴중인 상태
                stun_timer_bullet += Time.deltaTime;

                //Stun시간 지나면 Stun해제
                if (stun_timer_bullet > STUN_TIME_BULLET)
                {
                    stun_timer_bullet = 0.0f;
                    next_step = STEP.NONE;
                }
                break;

        }

        // 상태가 변화할 때의 처리
        if (this.step != this.next_step)
        {
            this.step = this.next_step;

            switch (this.step)
            {
                case STEP.NONE:                     // '대기' 상태 처리
                    Now.GetComponent<Animator>().SetBool("isRunning", false);
                    break;
                case STEP.MOVE:                     // '이동' 상태 처리
                    Now.GetComponent<Animator>().SetBool("isRunning", true);
                    break;
                case STEP.MOUSE_MOVE:
                    Now.GetComponent<Animator>().SetBool("isRunning", true);
                    break;
                case STEP.EATING:                   // '식사' 상태 처리
                    if (this.carried_item != null)
                    {
                        GameObject.Destroy(this.carried_item);  // 먹은 아이템을 없앱니다.
                    }
                    break;
                case STEP.STUN:
                    break;
            }
            this.step_timer = 0.0f;
        }

        // 상태 유지시 처리
        switch (this.step)
        {
            case STEP.NONE:                     // '대기' 상태 처리
                Now.GetComponent<Animator>().SetBool("isRunning", false);
                this.Pick_Or_Drop_Control();
                break;
            case STEP.MOVE:                     // 키보드 이동 상태 처리
                Now.GetComponent<Animator>().SetBool("isRunning", true);
                this.Move_Control();                // 이동 조작
                this.Pick_Or_Drop_Control();        // 수집 조작
                //Set_Spine_Animation("run", true, 1.0f);
                break;
            case STEP.MOUSE_MOVE:               // 마우스 이동 상태 처리
                Now.GetComponent<Animator>().SetBool("isRunning", true);
                this.Mouse_Move_Control();          // 이동 조작
                //Set_Spine_Animation("run", true, 1.0f);
                break;
        }


        // --------------------------- 여러가지 키 입력시 조작 ------------------------

        //스킬 조작
        do
        {
            if (this.key.skill_change_left || this.key.skill_change_right)
            {
                Skill_Change_Control();
                break;
            }

            if (this.key.skill_ON)              // 스킬을 가지고 있다면?
            {
                Skill_Use_Control();
            }

            if (this.key.skill_OFF)
            {
                Skill_Reset_Control();
            }
        } while (false);

        //횃불 조작
        if (this.key.lit_Fire)              // 불 지피기
        {
            StartCoroutine("litFire");
        }

        this.light_timer -= Time.deltaTime;
        if (this.light_timer <= 0.0f || Input.GetKeyDown(KeyCode.Alpha0))       // 불 끄기, 테스트용으로 0번을 끄는모드
        {
            StartCoroutine("goDarkness");
            this.light_timer = 30.0f;       // 30초마다 시야 감소
        }

    }


    public void InteractWithItem()
    {
        if (mInteractItem != null)
        {
            mInteractItem.OnInteract();

            if (mInteractItem is InventoryItemBase)
            {
                InventoryItemBase inventoryItem = mInteractItem as InventoryItemBase;
                inventory.AddItem(inventoryItem);
                inventoryItem.OnPickup();

                if (inventoryItem.UseItemAfterPickup)
                {
                    inventory.UseItem(inventoryItem);
                }
            }
        }

        hud.CloseMessagePanel();

        mInteractItem = null;
    }


}
