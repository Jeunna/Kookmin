using System;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InventoryManager : MonoBehaviour {

    void Awake()
    {
        int test_num = 0;

        List<Dictionary<string, object>> data = CSVReader.Read("Inventory");

        for (var i = 0; i < data.Count; i++)
        {
            test_num = Convert.ToInt32(data[i]["Item_num"]);
        }

    }

    // Use this for initialization
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {

    }
}
