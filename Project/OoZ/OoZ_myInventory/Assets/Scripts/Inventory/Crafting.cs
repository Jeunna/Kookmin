using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class Crafting : MonoBehaviour{

    public GameObject CraftingSystem;

    public void OpenSystem()
    {
        CraftingSystem.SetActive(!CraftingSystem.active);
    }


}
