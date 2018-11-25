using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class ItemDropHandler : MonoBehaviour, IDropHandler {

    public Inventory _Inventory;
    Collider2D collider;

    public void Start()
    {
        collider = _Inventory.GetComponent<Collider2D>();
    }


    public void OnDrop(PointerEventData eventData)
    {
        RectTransform invPanel = transform as RectTransform;

        if (collider.OverlapPoint(Input.mousePosition))
        {
            _Inventory.GetComponent<CanvasGroup>().blocksRaycasts = false;
            Debug.Log(_Inventory.GetComponent<CanvasGroup>().blocksRaycasts);

            InventoryItemBase item = eventData.pointerDrag.gameObject.GetComponent<ItemDragHandler>().Item;
            GameObject slotPos = eventData.pointerDrag.gameObject.GetComponent<ItemDragHandler>().mySlot;
            if (item != null)
            {
                item.OnDropScreen(_Inventory, slotPos);
                _Inventory.GetComponent<CanvasGroup>().blocksRaycasts = true;
            }
        }
        else if(!RectTransformUtility.RectangleContainsScreenPoint(invPanel, Input.mousePosition))
        {
            InventoryItemBase item = eventData.pointerDrag.gameObject.GetComponent<ItemDragHandler>().Item;
            if (item != null)
            {
                _Inventory.RemoveItem(item);
                item.OnDrop();
            }

        }
    }
}
