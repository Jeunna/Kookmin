using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum EItemType
{
    Default,
    Consumable,
    Weapon
}

public class InteractableItemBase : MonoBehaviour
{
    public EItemType ItemType;   // 아이템 타입

    public string Name;     // 아이템 이름

    public Sprite Image;    // 아이템 이미지

    public bool Used;       // 사용하고 있는 아이템 확인

    public int MaxCount;    // 겹칠 수 있는 최대 숫자

    //public void OnPickup();

    //public void OnDrop();

    //public void OnUse();

    public string InteractText = "Press Z to pickup the item";

    public virtual void OnInteractAnimation(Animator animator)
    {
        animator.SetTrigger("tr_pickup");
    }

    public virtual void OnInteract()
    {
    }

    public virtual bool CanInteract(GameObject other)
    {
        return true;
    }
}


public class InventoryItemBase : InteractableItemBase
{

    ////인벤토리에 넣을 아이템 setting
    //public virtual string Name
    //{
    //    get
    //    {
    //        return "_base item_";
    //    }
    //}

    //public Sprite _Image;
    //public Sprite Image
    //{
    //    get { return _Image; }
    //}

    //bool _Used;
    //public bool Used
    //{
    //    get { return _Used; }
    //    set { _Used = value; }
    //}

    //public int _Num;
    //public int Num
    //{
    //    get
    //    {
    //        return _Num;
    //    }
    //}

    public InventorySlot Slot
    {
        get; set;
    }

    public virtual void OnUse()
    {
        transform.localPosition = PickPosition;
        transform.localEulerAngles = PickRotation;
    }

    public virtual void OnDrop()
    {
        RaycastHit hit = new RaycastHit();
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        if (Physics.Raycast(ray, out hit, 1000))
        {
            gameObject.SetActive(true);
            gameObject.transform.position = hit.point;
            //gameObject.transform.eulerAngles = DropRotation;
        }
    }

    public virtual void OnDropScreen(Inventory _Inventory, GameObject slotPos)
    {
        //Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        //RaycastHit hit;
        //if (Physics.Raycast(ray, out hit))
        //{
        //    Debug.Log(hit.collider.gameObject.name);
        //    //gameObject.transform.parent = hit.transform.parent;
        //    //Debug.Log(hit.collider.name);
        //}s

        Vector2 mousePos = Input.mousePosition;

        RaycastHit2D hit = Physics2D.Raycast(mousePos, Vector2.zero);
        //if anything is collided
        if (hit.collider != null)
        {
            print(hit.collider.name);
            //if(slotPos.GetComponent<Collider2D>() != hit.collider)
            //{

            //}
        }
    
    }

    public virtual void OnPickup()
    {
        Destroy(gameObject.GetComponent<Rigidbody>());
        gameObject.SetActive(false);
    }

    public Vector3 PickPosition;

    public Vector3 PickRotation;

    public Vector3 DropRotation;

    public bool UseItemAfterPickup = false;
}
