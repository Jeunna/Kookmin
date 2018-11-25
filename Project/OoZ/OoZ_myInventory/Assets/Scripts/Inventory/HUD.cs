using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class HUD : MonoBehaviour {

    public Inventory Inventory;
    public GameObject MessagePanel;
    
    public Text MessageText;

    public GameObject elementalBubble;
    public Camera camera;
    public GameObject player;
    private Transform target;
    public bool isOpen = false;

    public float plusY = 0;
    public float plusZ = 0;
    public float plusAlpha = 3;
    Vector3 elementalPos;

    void Start ()
    {
        //scaler = GetComponent<CanvasScaler>();
        //scaler.uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;

        Inventory.ItemAdded += InventoryScript_ItemAdded;
        Inventory.ItemRemoved += Inventory_ItemRemoved;

        elementalBubble.SetActive(false);
        isOpen = false;
        //target = transform.Find("Player").GetComponent<Transform>();
        target = player.GetComponent<Transform>();

    }

    void Update()
    {
        if (isOpen)
        {
            Vector3 playerPos = target.position;
            playerPos.y += plusAlpha;
            Vector3 screenPos = camera.WorldToScreenPoint(playerPos);
            //Vector3 screenPos = camera.WorldToScreenPoint(target.position);
            float x = screenPos.x;
            //float y = target.position.y + plusY;

            //elementalText.transform.position = new Vector3(x, screenPos.y + plusY, elementalText.transform.position.z + plusZ);
            //Vector3 elementalPos = new Vector3(x, screenPos.y + plusY, elementalBubble.transform.position.z + plusZ);
            elementalPos.Set(x, screenPos.y + plusY, elementalBubble.transform.position.z + plusZ);
            elementalBubble.transform.position = elementalPos;
            //elementalBubble.transform.position = playerPos;
        }
    }

    // 아이템 추가
    private void InventoryScript_ItemAdded(object sender, InventoryEventArgs e)
    {
        Transform inventoryPanel = transform.Find("Inventory");

        int index = -1;
        foreach (Transform slot in inventoryPanel)
        {
            index++;

            // Border... Image 슬롯 이미지가져와서
            Transform imageTransform = slot.GetChild(0).GetChild(0);
            Transform textTransform = slot.GetChild(0).GetChild(1);
            Image image = imageTransform.GetComponent<Image>();
            Text txtCount = textTransform.GetComponent<Text>();
            ItemDragHandler itemDragHandler = imageTransform.GetComponent<ItemDragHandler>();

            // 해당 슬롯 이미지를 아이템 이미지로 바꿔줌
            if (index == e.Item.Slot.Id)
            {
                image.enabled = true;
                image.sprite = e.Item.Image;

                // 개수 증가
                int itemCount = e.Item.Slot.Count;
                if (itemCount > 1)
                    txtCount.text = itemCount.ToString();
                else
                    txtCount.text = "";


                // Store a reference to the item
                itemDragHandler.Item = e.Item;

                break;
            }
        }
    }

    // 아이템 지우기
    private void Inventory_ItemRemoved(object sender, InventoryEventArgs e)
    {
        Transform inventoryPanel = transform.Find("Inventory");

        int index = -1;
        foreach (Transform slot in inventoryPanel)
        {
            index++;

            Transform imageTransform = slot.GetChild(0).GetChild(0);
            Transform textTransform = slot.GetChild(0).GetChild(1);

            Image image = imageTransform.GetComponent<Image>();
            Text txtCount = textTransform.GetComponent<Text>();

            ItemDragHandler itemDragHandler = imageTransform.GetComponent<ItemDragHandler>();

            // We found the item in the UI
            if (itemDragHandler.Item == null)
                continue;

            // Found the slot to remove from
            if (e.Item.Slot.Id == index)
            {
                int itemCount = e.Item.Slot.Count;
                itemDragHandler.Item = e.Item.Slot.FirstItem;

                // 개수 변화로 인한 이미지 변경
                if (itemCount < 2)
                {
                    txtCount.text = "";
                }
                else
                {
                    txtCount.text = itemCount.ToString();
                }

                if (itemCount == 0)
                {
                    image.enabled = false;
                    image.sprite = null;
                }
                break;
            }

        }
    }

    // 키조작 UI 띄우기들
    private bool mIsMessagePanelOpened = false;

    public bool IsMessagePanelOpened
    {
        get { return mIsMessagePanelOpened; }
    }

    public void OpenMessagePanel(InteractableItemBase item)
    {
        MessagePanel.SetActive(true);

        Text mpText = MessagePanel.transform.Find("Text").GetComponent<Text>();
        mpText.text = item.InteractText;

        mIsMessagePanelOpened = true;
    }

    public void OpenMessagePanel(string text)
    {
        MessagePanel.SetActive(true);

        Text mpText = MessagePanel.transform.Find("Text").GetComponent<Text>();
        mpText.text = text;

        mIsMessagePanelOpened = true;
    }

    public void CloseMessagePanel()
    {
        MessagePanel.SetActive(false);

        mIsMessagePanelOpened = false;
    }

    public void OpenElementalBubble(string text)
    {
        elementalBubble.SetActive(true);

        Text elementalText = elementalBubble.transform.Find("Text").GetComponent<Text>();
        elementalText.text = text;

        isOpen = true;
    }

    public void CloseElementalBubble()
    {
        elementalBubble.SetActive(false);

        isOpen = false;
    }
}
