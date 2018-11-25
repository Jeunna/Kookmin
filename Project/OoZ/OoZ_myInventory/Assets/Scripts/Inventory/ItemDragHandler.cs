using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class ItemDragHandler : MonoBehaviour, IDragHandler, IEndDragHandler, IPointerDownHandler{

    public InventoryItemBase Item { get; set; }

    public Inventory inventory;

    public Image myImg;
    public Text mytext;

    public GameObject mySlot;

    void Start()
    {
        myImg = GetComponent<Image>();
        mytext = transform.parent.GetChild(1).GetComponent<Text>();
        mySlot = transform.parent.GetComponent<GameObject>();
    }

    // 처음 눌렀을 때
    public void OnPointerDown(PointerEventData eventData)
    {
        //if (Input.GetMouseButtonDown(1))
        //{
        //    //TODO : use item code
        //    return;
        //}

        inventory.EmptyImg.sprite = Item.Slot.FirstItem.Image;
        inventory.Img.transform.position = Input.mousePosition;
        //inventory.Img.SetActive(true);

        //myImg.enabled = false;
        //mytext.enabled = false;
    }

    // 누르고 드래그 할 때
    public void OnDrag(PointerEventData eventData)
    {
        transform.position = Input.mousePosition;
        //inventory.Img.transform.position = Input.mousePosition;
    }

    // 드래그를 끝냈을 때
    public void OnEndDrag(PointerEventData eventData)
    {
        // TODO : Swap
        transform.localPosition = Vector3.zero;
    }

    // 마우스 버튼을 땠을 때
    //public void OnPointerUp(PointerEventData eventData)
    //{
    //    Debug.Log("땜");
    //    Img.SetActive(false);
    //    myImg.enabled = true;
    //    mytext.enabled = true;
    //}
}
