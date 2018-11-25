using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class InventorySlot
{
    // 슬롯을 스택으로 만듦, 스택 메모리 할당
    private Stack<InventoryItemBase> slot = new Stack<InventoryItemBase>();

    public Text text;   // 아이템에 개수를 표현해줄 텍스트
    private bool isSlot = false;    // 처음엔 슬롯이 비어있다

    private int mId = 0;

    public InventorySlot(int id)
    {
        mId = id;
    }

    public int Id
    {
        get { return mId; }
    }

    public void AddItem(InventoryItemBase item)
    {
        item.Slot = this;
        slot.Push(item);
    }


    // 슬롯에 존재하는 아이템이 뭔지 반환.
    public InventoryItemBase FirstItem
    {
        get
        {
            if (IsEmpty)
                return null;

            return slot.Peek();
        }
    }

    public bool IsStackable(InventoryItemBase item)
    {
        if (IsEmpty)
            return false;

        InventoryItemBase first = slot.Peek();

        if (first.Name == item.Name)
            return true;

        return false;
    }

    public bool IsEmpty
    {
        get { return Count == 0; }
    }

    public int Count
    {
        get { return slot.Count; }
    }

    public bool Remove(InventoryItemBase item)
    {
        if (IsEmpty)
            return false;

        InventoryItemBase first = slot.Peek();
        if (first.Name == item.Name)
        {
            slot.Pop();
            return true;
        }
        return false;
    }

}
